
# SYSTEM IMPORTS
import os
import platform
import sys


# PYTHON PROJECT IMPORTS
sys.path.append("scripts")  # now we can import modules from <currentDirectory>/scripts
import LocalROSBuildRules
import GlobalROSBuildRules
import Utilities
import FileSystem



if __name__ == "__main__":
    localBuild = None
    customCommands = Utilities.parseCommandLine(sys.argv[1:])
    print(customCommands)
    if "projects" not in customCommands[1]:
        customCommands[1]["projects"] = ["classifiers"]

    help = False
    for projectName in customCommands[1]["projects"]:
        localBuild = LocalROSBuildRules.LocalROSBuild(projectName)
        if "help" in customCommands[0]:
            localBuild.help()
            help = True
        else:
            localBuild.run(customCommands)
    if help:
        GlobalROSBuildRules.help()
