#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>

enum eCommand
{
	FILE_SIZE,
	STAT_ALL,
	FILE_TYPE,
	CHMOD,
	CWD,
	MKDIR,
	RMDIR,
	LIST,
	HARD_LINK,
	SOFT_LINK,
	HELP,
	EXIT,
};

void setCommand(int commandIndex, char* command, char* commandText);
int initialize();
int switchCommand(int command);
int progress();
int progressCommand(char* command);
void release();

char g_Commands[1024][1024];
char g_CommandTexts[1024][1024];

char* getInputString(char* print);
int getInputNumber(char* print);

void readInotifyEvent();

void commandFileSize();
void commandStatAll();
void commandHelp();
void commandFileType();
void commandChangeMod();
void commandCWD();
void commandMKDir();
void commandRMDir();
void commandList();
void commandHardLink();
void commandSoftLink();		//symbolic link

int main(int argc, char* argv[])
{
    int returnValue = 0;

    if(argc > 0)
    {
        if(initialize())
        {
            returnValue = progressCommand(argv[0]);
            release();
        }
    }

//    printf("returnValue :%d\n", returnValue);
    return returnValue;
}

void setCommand(int commandIndex, char* command, char* commandText)
{
	strcpy(g_Commands[commandIndex], command);
	strcpy(g_CommandTexts[commandIndex], commandText);
}

int initialize()
{
	setCommand(FILE_SIZE, "fs", "getfilesize");
	setCommand(STAT_ALL, "statall", "stat all");
	setCommand(HELP, "help", "help Command");
	setCommand(EXIT, "quit", "quit command");
	setCommand(FILE_TYPE, "ft", "file type command");
	setCommand(CHMOD, "chmod", "change mode");
	setCommand(CWD, "cwd", "current directory");
	setCommand(MKDIR, "mkdir", "make directory");
	setCommand(RMDIR, "rmdir", "remove direcotry");
	setCommand(LIST, "list", "directory in list");
	setCommand(HARD_LINK, "hdlk", "hard link");
	setCommand(SOFT_LINK, "stlk", "soft link");

	return 1;
}

int progress()
{
	int i = 0;
	int returnValue;
	char* buffer = getInputString(">> ");

	for(i = 0; i <= EXIT; ++i)
	{
		if(strcmp(buffer, g_Commands[i]) == 0)
		{
			returnValue = switchCommand(i);
			break;
		}
	}

	free(buffer);

	return returnValue;
}

int progressCommand(char* command)
{
    int returnValue = 0;

    for(int i = 0; i <= EXIT; ++i)
    {
        if(strcmp(command, g_Commands[i]) == 0)
        {
            returnValue = 1;
            switchCommand(i);
            break;
        }
    }

    return returnValue;
}

int switchCommand(int command)
{
	switch(command)
	{
		case FILE_SIZE :
			commandFileSize();
			break;

		case STAT_ALL :
			commandStatAll();
			break;

		case FILE_TYPE :
			commandFileType();
			break;

		case CHMOD :
			commandChangeMod();
			break;

		case CWD :
			commandCWD();
			break;

		case HELP :
			commandHelp();
			break;

		case MKDIR :
			commandMKDir();
			break;

		case RMDIR :
			commandRMDir();
			break;

		case LIST :
			commandList();
			break;
	
		case HARD_LINK :
			commandHardLink();
			break;

		case SOFT_LINK :
			commandSoftLink();
			break;

		case EXIT :
			return 0;
	}

	return 1;
}

void release()
{
	
}

char* getInputString(char* print)
{
	char* buffer = (char*)malloc(1024);
	printf(print);
	scanf("%s", buffer);

	return buffer;
}

int getInputNumber(char* print)
{
	int number;

	printf(print);
	scanf("%d", &number);

	return number;	
}

void commandFileSize()
{
	//page 326 example
	char* buffer;
	struct stat sb;
	int ret;

	buffer = getInputString("FileName : ");

	ret = stat(buffer, &sb);

	printf("%s is %ld bytes\n", buffer, sb.st_size);

	free(buffer);
}

void commandHelp()
{
	int i = 0;

	for(i = 0; i <= EXIT; ++i)
	{
		printf("%s = %s\n", g_Commands[i], g_CommandTexts[i]);
	}
}

void commandStatAll()
{
	char* buffer;
	struct stat sb;
	int ret;

	buffer = getInputString("FileName : ");

	ret = stat(buffer, &sb);

	/*
	struct stat {
		dev_t     st_dev;     // ID of device containing file 
		ino_t     st_ino;     // inode number 
		mode_t    st_mode;    // protection 
		nlink_t   st_nlink;   // number of hard links 
		uid_t     st_uid;     // user ID of owner 
		gid_t     st_gid;     // group ID of owner 
		dev_t     st_rdev;    // device ID (if special file) 
		off_t     st_size;    // total size, in bytes 
		blksize_t st_blksize; // blocksize for file system I/O 
		blkcnt_t  st_blocks;  // number of 512B blocks allocated 
		time_t    st_atime;   // time of last access 
		time_t    st_mtime;   // time of last modification 
		time_t    st_ctime;   // time of last status change 
	};
	*/

	printf("st_dev : %ld\n", sb.st_dev);
	printf("st_ino : %ld\n", sb.st_ino);
	printf("st_mode : %ld\n", sb.st_mode);
	printf("st_link : %ld\n", sb.st_nlink);
	printf("st_uid : %ld\n", sb.st_uid);
	printf("st_gid : %ld\n", sb.st_gid);
	printf("st_rdev : %ld\n", sb.st_rdev);
	printf("st_size : %ld\n", sb.st_size);
	printf("st_blksize : %ld\n", sb.st_blksize);
	printf("st_blocks : %ld\n", sb.st_blocks);
	printf("st_atime : %ld\n", sb.st_atime);
	printf("st_mtime : %ld\n", sb.st_mtime);
	printf("st_ctime : %ld\n", sb.st_ctime);
	
	free(buffer);
}

void commandFileType()
{
	char* buffer;
	struct stat sb;
	int ret;

	buffer = getInputString("FileName : ");

	ret = stat(buffer, &sb);

	switch(sb.st_mode & S_IFMT) {
		case S_IFBLK :
			printf("block device node\n");
			break;

		case S_IFCHR :
			printf("character device node\n");
			break;

		case S_IFDIR :
			printf("directory\n");
			break;

		case S_IFIFO :
			printf("FIFO\n");
			break;

		case S_IFLNK :
			printf("symbolic links\n");
			break;

		case S_IFREG :
			printf("regular file \n");
			break;

		case S_IFSOCK :
			printf("socket\n");
			break;

		default :
			printf("unknown\n");
			break;
	}
	free(buffer);
}

void commandChangeMod()
{
	char* buffer;
	int mod;
	int ret;

	buffer = getInputString("FileName : ");
	mod = getInputNumber("Mod : ");

	ret = chmod(buffer, mod);

	if(ret)
		perror("chmod");

	free(buffer);
}

void commandCWD()
{
	char currentDirectoryBuffer[1024];
	int bufferSize;
	int ret;

	if(!getcwd(currentDirectoryBuffer, 1024)){
		perror("getcwd");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", currentDirectoryBuffer);	
}

void commandMKDir()
{
	char* buffer;
	mode_t mode;
	int ret;

	buffer = getInputString("makedirecotry : ");
	mode = getInputNumber("Mode : ");

	ret = mkdir(buffer, (mode & 0177));

	free(buffer);

	if(ret == -1)
		perror("mkdir");
}

void commandRMDir()
{
	char* buffer;
	int ret;

	buffer = getInputString("removeDirectory : ");
	
	ret = rmdir(buffer);

	free(buffer);

	if(ret)
		perror("rmdir");
}

void commandList()
{
	struct dirent *entry;
	int ret = 1;
	DIR* dir;
	char* buffer;

	buffer = getInputString("DirectoryPath : ");
	
	dir = opendir(buffer);

	free(buffer);

	errno = 0;
	while((entry = readdir(dir)) != NULL)
	{
		printf("%s\n", entry->d_name);
	}

	if(errno && !entry)
		perror("readdir");

	closedir(dir);

	return ret;
}

void commandHardLink()
{
	char* newPath;
	char* oldPath;
	int ret;

	oldPath = getInputString("old path : ");
	newPath = getInputString("new path : ");

	ret = link(oldPath, newPath);

	free(oldPath);
	free(newPath);

	if(ret)
		perror("link");
}

void commandSoftLink()
{
	char* newPath;
	char* oldPath;
	int ret;	

	oldPath = getInputString("old path : ");
	newPath = getInputString("new path : ");

	ret = symlink(oldPath, newPath);

	free(oldPath);
	free(newPath);

	if(ret == -1)
		perror("symlink");
}
