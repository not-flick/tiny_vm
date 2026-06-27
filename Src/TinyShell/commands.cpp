#include "commands.h"

namespace tinyvm
{
    namespace
    {
        void RegisterCommand(CommandMap& commands, const std::string& name, const std::string& description, void (*handler)(ShellState&, const std::vector<std::string>&))
        {
            commands.emplace(name, Command{name, description, handler});
        }
    }

    CommandMap CreateCommandRegistry()
    {
        CommandMap commands;
        RegisterCommand(commands, "help", "List available commands.", HelpCommand);
        RegisterCommand(commands, "clear", "Clear the terminal.", ClearCommand);
        RegisterCommand(commands, "exit", "Exit TinyShell.", ExitCommand);
        RegisterCommand(commands, "pwd", "Print the current directory.", PwdCommand);
        RegisterCommand(commands, "cd", "Change the current directory.", CdCommand);
        RegisterCommand(commands, "ls", "List directory contents.", LsCommand);
        RegisterCommand(commands, "mkdir", "Create a directory.", MkdirCommand);
        RegisterCommand(commands, "touch", "Create an empty file.", TouchCommand);
        RegisterCommand(commands, "rm", "Delete a file.", RmCommand);
        RegisterCommand(commands, "rmdir", "Delete an empty directory.", RmdirCommand);
        RegisterCommand(commands, "cat", "Print file contents.", CatCommand);
        RegisterCommand(commands, "echo", "Print text.", EchoCommand);
        RegisterCommand(commands, "whoami", "Print the current username.", WhoamiCommand);
        RegisterCommand(commands, "hostname", "Print the hostname.", HostnameCommand);
        RegisterCommand(commands, "version", "Print the shell version.", VersionCommand);
        RegisterCommand(commands, "history", "Print command history.", HistoryCommand);
        RegisterCommand(commands, "alias", "Create or list command aliases.", AliasCommand);
        RegisterCommand(commands, "unalias", "Remove a command alias.", UnaliasCommand);
        RegisterCommand(commands, "set", "Set an environment variable.", SetCommand);
        RegisterCommand(commands, "unset", "Unset an environment variable.", UnsetCommand);
        return commands;
    }
}
