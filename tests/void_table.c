/* void_table.c */
/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                     Version 2, December 2004
 * 
 *  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 * 
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 * 
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 */
#include <acknex.h>

#define __namespace(nspc) {}

__namespace(void_table) {
	#define MAX_COMMANDS          10
	#define MAX_COMMAND_LENGTH    30
	
	int __CommandTable_pos = 0;
	char *__CommandTable_command[MAX_COMMANDS];
	void *__CommandTable__action[MAX_COMMANDS];
	
	void command_table_add( const char *command, void *__action )
	{	
		if(__CommandTable_pos < MAX_COMMANDS - 1)
		{
			__CommandTable_command[__CommandTable_pos] = (char *) malloc(sizeof(char) * MAX_COMMAND_LENGTH);
			strcpy(__CommandTable_command[__CommandTable_pos], command);
			__CommandTable__action[__CommandTable_pos] = __action;
			__CommandTable_pos++;
		}
	}
	
	void command_table_call(int id)
	{
		printf("%s", __CommandTable_command[id]);
		
		void ___action();
		___action = __CommandTable__action[id];
		___action();
	}
}

void f() 
{
	printf("%s", "f()");
}

void g()
{
	printf("%s", "g()");
}

int main( int argc, char **argl )
{
	command_table_add("g() invoked", g );
	command_table_add("f() invoked", f );
	
	command_table_call(0);
	command_table_call(1);
	// command_table_call(2); // Crash
	
	sys_exit(0);
}
