#include "commands.h"

namespace tinyvm
{
    CommandMap CreateCommandRegistry()
    {
        return CommandMap{
            {"help", {"help", "List available commands.", HelpCommand}},
            {"clear", {"clear", "Clear the terminal.", ClearCommand}},
            {"exit", {"exit", "Exit TinyShell.", ExitCommand}},
            {"pwd", {"pwd", "Print the current directory.", PwdCommand}},
            {"cd", {"cd", "Change the current directory.", CdCommand}},
            {"ls", {"ls", "List directory contents.", LsCommand}},
            {"mkdir", {"mkdir", "Create a directory.", MkdirCommand}},
            {"touch", {"touch", "Create an empty file.", TouchCommand}},
            {"rm", {"rm", "Delete a file.", RmCommand}},
            {"rmdir", {"rmdir", "Delete an empty directory.", RmdirCommand}},
            {"cat", {"cat", "Print file contents.", CatCommand}},
            {"echo", {"echo", "Print text.", EchoCommand}},
            {"whoami", {"whoami", "Print the current username.", WhoamiCommand}},
            {"hostname", {"hostname", "Print the hostname.", HostnameCommand}},
            {"version", {"version", "Print the shell version.", VersionCommand}},
        };
    }
}
