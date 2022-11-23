#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <signal.h>
#include <map>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define DEFAULT_PROMPT_NAME ("smash")

class Command {
protected:
    size_t m_args_num;
    char* m_args[COMMAND_MAX_ARGS];

public:
    Command(const char* cmd_line);
    virtual ~Command();
    virtual void execute() = 0;
};

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char* cmd_line)
        : Command(cmd_line)
    {
    }
    virtual ~BuiltInCommand(){}
};

class ExternalCommand : public Command {
private:
    std::string m_cmd_line;

public:
    ExternalCommand(const char* cmd_line)
        : Command(cmd_line)
        , m_cmd_line(cmd_line)
    {
    }
    virtual ~ExternalCommand(){}
    void execute() override;
};

class PipeCommand : public Command {
    // TODO: Add your data members
public:
    PipeCommand(const char* cmd_line);
    virtual ~PipeCommand() { }
    void execute() override;
};

class RedirectionCommand : public Command {
    // TODO: Add your data members
public:
    explicit RedirectionCommand(const char* cmd_line);
    virtual ~RedirectionCommand() { }
    void execute() override;
    // void prepare() override;
    // void cleanup() override;
};

class ChangePromptCommand : public BuiltInCommand {
public:
    ChangePromptCommand(const char* cmd_line)
        : BuiltInCommand(cmd_line)
    {
    }
    virtual ~ChangePromptCommand() { }
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char* cmd_line)
        : BuiltInCommand(cmd_line)
    {
    }
    virtual ~ShowPidCommand() { }
    void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char* cmd_line)
        : BuiltInCommand(cmd_line)
    {
    }
    virtual ~GetCurrDirCommand() { }
    void execute() override;
};

class ChangeDirCommand : public BuiltInCommand {
public:
    ChangeDirCommand(const char* cmd_line)
        : BuiltInCommand(cmd_line)
    {
    }
    virtual ~ChangeDirCommand() { }
    void execute() override;
};

class JobsList;
class QuitCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    QuitCommand(const char* cmd_line, JobsList* jobs);
    virtual ~QuitCommand() { }
    void execute() override;
};

class JobEntry {
public:
    std::string cmd_line;
    int job_id;
    pid_t pid;
    time_t time_epoch;
    bool stopped;
    JobEntry(){}
    JobEntry(std::string cmd_line, pid_t pid);
};

class JobsList {
private:
    std::map<int, JobEntry> m_job_list;

public:
    JobsList()
        : m_job_list()
    {
    }
    JobsList(JobsList const&) = delete; // disable copy ctor
    void operator=(JobsList const&) = delete; // disable = operator
    void addJob(const JobEntry& job);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    JobEntry* getJobById(int jobId);
    void removeJobById(int jobId);
    JobEntry* getLastJob(int* lastJobId);
    JobEntry* getLastStoppedJob(int* jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    JobsCommand(const char* cmd_line, JobsList* jobs);
    virtual ~JobsCommand() { }
    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    ForegroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~ForegroundCommand() { }
    void execute() override;
};

class BackgroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    BackgroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~BackgroundCommand() { }
    void execute() override;
};

class TimeoutCommand : public BuiltInCommand {
    /* Optional */
    // TODO: Add your data members
public:
    explicit TimeoutCommand(const char* cmd_line);
    virtual ~TimeoutCommand() { }
    void execute() override;
};

class FareCommand : public BuiltInCommand {
    /* Optional */
    // TODO: Add your data members
public:
    FareCommand(const char* cmd_line);
    virtual ~FareCommand() { }
    void execute() override;
};

class SetcoreCommand : public BuiltInCommand {
    /* Optional */
    // TODO: Add your data members
public:
    SetcoreCommand(const char* cmd_line);
    virtual ~SetcoreCommand() { }
    void execute() override;
};

class KillCommand : public BuiltInCommand {
    /* Bonus */
    // TODO: Add your data members
public:
    KillCommand(const char* cmd_line, JobsList* jobs);
    virtual ~KillCommand() { }
    void execute() override;
};

class SmallShell {
public:
    bool old_pwd;
    std::string old_path;
    // if set to -1 the shell don't need to wait
    volatile sig_atomic_t waitpid;
    volatile sig_atomic_t job_stopped;
    JobEntry foreground_job;
    JobsList jobs_list;

private:
    std::string m_prompt_name;
    SmallShell()
        : m_prompt_name(DEFAULT_PROMPT_NAME)
        , old_pwd(false)
        , waitpid(-1)
        , foreground_job()
    {
    }

public:
    Command* CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&) = delete; // disable copy ctor
    void operator=(SmallShell const&) = delete; // disable = operator
    static SmallShell& getInstance() // make SmallShell singleton
    {
        static SmallShell instance;
        return instance;
    }
    ~SmallShell() { }
    void executeCommand(const char* cmd_line);
    void changePromptName(const std::string&& prompt_name = DEFAULT_PROMPT_NAME);
    void printPrompt();
};

#endif // SMASH_COMMAND_H_
