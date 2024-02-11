#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <csignal>


int main()
{
	char *env[] = {const_cast<char*>("QUERY_STRING=NUM=4"), NULL};
	
	char *const args[] = {const_cast<char*>("perl"), const_cast<char*>("cgi-bin/prime.pl"), NULL};
	
	execve("/usr/bin/perl", const_cast<char *const *>(args), env);


	return (0);
}