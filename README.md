# CacheClear
The files included here will enable new terminal commands designed to assist with file management, specifically to manage storage on your local machine, and help free up space.

## Installation and Setup
### Prerequisites
* C++17 or later
* A C++ compiler (e.g., GCC, Clang)
* zip utility installed on your system (or equivalent for creating zip files)

### Installation
* Clone the repository or download the source files.
* Navigate to the scripts directory.
* Run the following command.

```chmod +x auto-archive.sh clear-temp.sh list-files.sh```

* Add the bin and scripts directories to the PATH file.
* Open a Terminal and run the following commands.

```nano ~/.bashrc```

Or for zshrc

```nano ~/.zshrc```

Add the following lines to the file.

```
export PATH=$PATH:/path/to/your/project/bin
export PATH=$PATH:/path/to/your/project/scripts
```

## Available Commands
All commands can now be run from any directory on the machine.

### list-files
The purpose of this command is to list all the files and directories in your current working directory, along with the amount of space they are using.

Syntax:

```list-files.sh max_depth max_files max_size```

The optional max_depth, max_files, and max_size parameters specify the maximum depth the function should recurse to when exploring subdirectories to calculate a directory size. The default values for these parameters is -1, meaning there is no limit to the recursion depth.

### clear-temp
The purpose of this command is to clear temporary files that are taking extra space within your current directory. This command will clear all files in the current directory and all subdirectories that have the type ```.tmp```, ```.log```, and ```.cache```. ```.tmp```. Exclusions to this are ```.tmp``` files that have been modified in the last 24 hours, and the following special directories. /etc, /bin, /usr, /lib, /opt, and /sbin.

Syntax:

```clear-temp.sh --force```

The optional ```--force``` parameter will prevent the system from requesting permission before every file deletion, and will delete all files fitting the above criteria.

### auto-archive
The purpose of this command is to compress unused files within your system to save space. This command recurses through your current directory and all subdirectories, and at each level compresses all files into an ```archive.zip``` file, maintaing the structure of your directories.

Syntax:

```auto-archive num_days```

The optional num_days parameter specifies how many days without access the system should consider unused, and archive. The default for this parameter is 24 days.