
import platform
import os


class LocalEnvironment(object):
    def __init__(self, projectSpecificEnvVars={}):
        self._static_environment_variables = {
            "C_CACHE_DIR": "" if platform.system() == "Windows" else "",
            # other static environment variables
        }
        self._project_specific_static_env_vars = projectSpecificEnvVars

    def injectStaticEnvironmentVariables(self):
        # print("Injecting static environment variables")
        env = os.environ
        for item in self._static_environment_variables.items():
            if item[0] not in env:
                env[item[0]] = item[1]

    def injectProjectSpecificEnvironmentVariables(self):
        # print("Injecting project specific environment variables")
        env = os.environ
        for item in self._project_specific_static_env_vars.items():
            if item[0] not in env:
                env[item[0]] = item[1]

    def injectDynamicEnvironmentVariables(self):
        env = os.environ
        toAdd = env.get("DYNAMIC_ENV_VARS_FILE")
        if toAdd is not None and os.path.exists(toAdd):
            # print("Injecting dynamic environment variables")
            with open(toAdd) as file:
                splitLine = None
                keyToAdd = None
                valueToAdd = None
                lineNum = 1
                for line in file:
                    splitLine = line.split('=')
                    if len(splitLine) == 2:
                        keyToAdd = splitLine[0].strip()
                        valueToAdd = splitLine[1].strip()
                        if keyToAdd not in env:
                            env[keyToAdd] = valueToAdd
                    else:
                        raise Exception("invalid syntax in [%s] at line [%s] (line 1 is beginning of file)" %
                                        (toAdd, lineNum))
                    lineNum += 1
        else:
            print("For potential environment optimization, consider adding\n" +
                  "a file of key=value pairs. These will be injected as environment\n" +
                  "variables unique to this project and will only exist for the runtime\n" +
                  "of this project")

    def injectAllEnvironmentVariables(self):
        # print("Injecting all environment variables")
        self.injectStaticEnvironmentVariables()
        self.injectProjectSpecificEnvironmentVariables()
        self.injectDynamicEnvironmentVariables()
