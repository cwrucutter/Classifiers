
# SYSTEM IMPORTS
import inspect
import os
import platform
import shutil
import subprocess
import sys
import traceback

# PYTHON PROJECT IMPORTS

def failExecution(errorMsg):
    print("Failed Build Stack:")
    traceback.print_stack(file=sys.stdout)

    print("********************\n* FAILURE %s\n********************\n" % errorMsg)
    sys.exit(1)


def PFork(appToExecute=None, argsForApp=[], wd=None, failOnError=False, environment={}):

    # a list that will contain all arguments with the application to
    # execute as the first element. This is to invoke Popen with,
    # which takes as an argument the commands that will be executed.
    app_and_args = list(argsForApp)
    if appToExecute is not None:
        app_and_args.insert(0, appToExecute)

    # get the current environment and update it with custom environment
    # variables injected into this method.
    real_environment = dict(os.environ)
    real_environment.update(environment)

    # execute the code. This method does not use the shell for safety
    # reasons. It will store the result in a pipe and default write all
    # errors to the console (later we can ignore the pipe and safe time
    # on the builds). Tidbit: "Popen" stands for "process open" also known as "fork."
    # note that this DOES NOT create a new console process (no new window will pop up)
    # when this executes.
    print("Executing %s" % ' '.join(app_and_args))
    childProcess = subprocess.Popen(app_and_args, cwd=wd, shell=False, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, env=real_environment, bufsize=1)

    # (for now) write the messages of the process execution to the console.
    for line in iter(childProcess.stdout.readline, b''):
        print line.rstrip() + '\n',

    # wait for childProcess and let all childProcess values settle
    # (like the returncode variable)
    childProcess.communicate()

    print("RETURN CODE FOR APP %s IS: %s" % (appToExecute, childProcess.returncode))

    if childProcess.returncode != 0 and failOnError:
        failExecution("PFork failed to execute [%s]" % ' '.join(app_and_args))
    return childProcess.returncode


def mkdir(dir):

    # only create the directory if it doesn't exist.
    # we should only run into the directory already existing
    # DURING a build, as all created directories will be
    # deleted at the beginning of each build.
    if not os.path.exists(dir):
        os.makedirs(dir)


def rmTree(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    else:
        os.unlink(path)  # same as os.remove(path)


def copyTree(srcPath, destPath):
    if os.path.isdir(srcPath):
        if os.path.isdir(destPath):
            failExecution("cannot copy directory [%s]: %s exists!" %
                          (srcPath, destPath))
        elif os.path.isfile(destPath):
            failExecution("Cannot copy directory [%s]: %s is a File!" %
                          (srcPath, destPath))
        shutil.copytree(srcPath, destPath)
    else:
        shutil.copy2(srcPath, destPath)  # copy2() copies file metaData


# this is no longer windows specific
def getProcessorInfo():
    machine = platform.machine().lower()
    if "x86_64" in machine:
        machine = "x64"
    elif "x86" in machine:
        machine = "x86_arm"
    elif "aarch64" in machine:
        machine = "aarch64"
    elif "x64" in machine:
        machine = "x64"
    elif "amd64" not in machine:
        failExecution("unsupported processor: %s" % machine)
    return machine
    '''
    # this is windows specific
    if platform.system() == "Windows":
        return getWindowsRegistryValue(
            "HKEY_LOCAL_MACHINE",
            "SYSTEM\\CurrentControlSet\Control\\Session Manager\\Environment",
            "PROCESSOR_ARCHITECTURE"
        ).lower()
    else:
        failExecution("Cannot get processor info on Unix based system")
    '''

'''
# this is windows specific
def getWindowsRegistryValue(key, subkey, value):
    if platform.system() == "Windows":
        key = getattr(_winreg, key)
        handle = _winreg.OpenKey(key, subkey)

        # this returns (value, type) where the value
        # is the value of the processor and the type
        # is the registry type of the value
        return _winreg.QueryValueEx(handle, value)[0]
    else:
        failExecution("Cannot get registry values on Unix based system")
'''

def getVisualStudioVersion():
    return int(os.environ.get('VS_VERSION')) if os.environ.get('VS_VERSION') is not None else 14

def formatVisualStudioVersion(visualStudioVersion=14):
    return "%s 20%s" % (visualStudioVersion, visualStudioVersion + 1)
    

def getMachineBits():
    if "64" in getProcessorInfo():
        return "x64"
    else:
        return "x86"


def parseCommandLine(commandLine):
    parsedCommandedValues = {}
    parsedCommandedMethods = []
    key = None
    projectFlag = False
    for command in commandLine:
        if command == "-projects":
            key = command.replace('-', '', 1)
            parsedCommandedValues[key] = []
            projectFlag = True
        elif '-' in command:
            key = command.replace('-', '', 1)
            projectFlag = False
        elif key is not None:
            if projectFlag is True:
                parsedCommandedValues[key].append(command)
            else:
                parsedCommandedValues[key] = command
                key = None
        else:
            parsedCommandedMethods.append(command)
    return (parsedCommandedMethods, parsedCommandedValues)


def PForkWithVisualStudio(appToExecute=None, argsForApp=[], wd=None, environment={}):
    appAndArgs = list(argsForApp)
    if appToExecute is not None:
        appAndArgs.insert(0, appToExecute)

    # there will be an environment variable set externally called
    # VS_VERSION that will declare the version of Visual Studio's Compiler
    # you want this build to use. Defaults to 2014
    visualStudioPathBase = os.environ.get('VS%s0COMNTOOLS' % (
                                          os.environ.get('VS_VERSION')
                                          if os.environ.get('VS_VERSION') is not None else '14'
                                          ))

    # this is the fully qualified path to vcvarsall.bat
    visualStudioUtilsPath = (
        os.path.join(visualStudioPathBase, '..', '..', 'VC', 'vcvarsall.bat')
        if visualStudioPathBase is not None else
        (
            failExecution('No Visual Studio Compiler present on system')
        )
    )

    # we want to execute the command in the appropriate working directory.
    # so we will change into that directory at the beginning of the command
    # execution.
    if wd is not None:
        wd = os.path.abspath(wd)
        appAndArgs.insert(0, ' cd "%s" && ' % wd)

    # this is important. We want to execute vcvarsall.bat so that the following
    # commands execute in the visual studio environment. vcvarsall.bat prints lots
    # of information and is optimized if we tell it what the processor architecture
    # is. So, we give it the processor architecture and redirect all output to 'nul'
    # which means we won't see it. Then, we add all the commands we want to execute.
    cmdString = '("%s" %s>nul)&&%s' % (visualStudioUtilsPath,
                                       getProcessorInfo(),
                                       ' '.join(appAndArgs))

    # allows additional environment variables to be set / altered.
    realEnv = dict(os.environ)
    realEnv.update(environment)
    print cmdString

    # fork a child process to execute out commands, piping the output to stdout (eventually)
    # and applying the environment given.
    childProcess = subprocess.Popen(cmdString, cwd=wd, shell=True, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, env=realEnv, bufsize=1)

    for line in iter(childProcess.stdout.readline, b''):
        print line.rstrip() + '\n'

    # this waits for the childProcess to finish its execution.
    # this is necessary to allow the childProcess variables to settle,
    # most importantly the returncode variable.
    childProcess.communicate()

    # fail execution if there was an error.
    if childProcess.returncode != 0:
        failExecution('execute with visual studio failed with returncode [%s]' % childProcess.returncode)


# this is a nifty function to attempt to call any callable object with its desired arguments.
def call(callable, args):
    # we need to build a dictionary of all the arguments that the callable object requires
    # to execute
    callableArgs = {}

    # inspect is a cool module that is used to analyze a callable object at runtime.
    # here we are getting the set of parameters that "callable" has in its method/function
    # signature. inspect has converted the parameter names to strings.
    for arg in inspect.getargspec(callable).args:
        if arg in args:
            callableArgs[arg] = args[arg]

    # note that at this point, callableArgs may NOT contain all arguments in "callables"
    # method/function signature. This is ok. Python allows default values to be specified
    # so we will hope that the arguments NOT contained in "callableArgs" are default parameters.
    # An example of this is if "callable" is a method. Methods in Python require the first
    # parameter to be "self" referring to the object that the method belongs to. inspect will
    # return "self" as a parameter, which we cannot provide, but will be provided by the Python
    # interpreter when executed.

    # this is known as variable unpacking. We are taking the dictionary, and unpacking
    # it, which the interpreter treats as a parameter list in this context. It will build
    # a final parameter list combining the default values of the method/function and the ones
    # we have provided and call the method/function with it. This throws an error if the total
    # parameter list does not match the callable's signature EXACTLY. This is why we needed
    # to construct "callableArgs" in the first place, to filter out excess arguments.
    callable(**callableArgs)
    return True
