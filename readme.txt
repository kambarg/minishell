test
===memory leaks checks with valgrind================
To check for memory leaks run:
make leaks

===readline and history libs========================
In modern compilers, using an undeclared function (rl_clear_history) leads to an error.
On Linux, Readline is installed and provides rl_clear_history(), so no error occurs.
MacOS provides BSD libedit by default, which is not fully compatible with GNU Readline.
rl_clear_history() is missing, but clear_history() exists.

== Install GNU Readline on MacOS (with sudo) ==
brew install readline

== Compile and Install GNU Readline on MacOS (without sudo) ==
cd $HOME
curl -O https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
tar -xzf readline-8.2.tar.gz
cd readline-8.2
./configure --prefix=$HOME/.local
make -j$(nproc)   # Compile using multiple coresAdd
make install

Now Readline is installed in $HOME/.local

Add this to your ~/.zshrc:
export PATH="$HOME/.local/bin:$PATH"
export LD_LIBRARY_PATH="$HOME/.local/lib:$LD_LIBRARY_PATH"
export CFLAGS="-I$HOME/.local/include"
export LDFLAGS="-L$HOME/.local/lib"
export PKG_CONFIG_PATH="$HOME/.local/lib/pkgconfig"

Apply changes:
source ~/.zshrc

Modify your Makefile to use your local Readline:
CFLAGS += -I$(HOME)/.local/include
LDFLAGS += -L$(HOME)/.local/lib -lreadline


===exit code/ exit status handling========================

in executor.c:
...
/* Wait for all child processes to complete */
 	last_status = 0;
/* Wait for the last command specifically to get its exit status */
	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		
		/* Manual implementation of status checking without macros */
		/* or */
		/* Macros implementation */
			
		/* Then wait for any other remaining children */
		while (waitpid(-1, NULL, 0) > 0)
			;
	}

==Macros implementation==
if (WIFEXITED(status))
	last_status = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
	last_status = 128 + WTERMSIG(status);

macros defined in <sys/wait.h>. Read manual for details:
$ man waitpid

==Manual implementation==
if ((status & 0x7f) == 0) /* Process exited normally (equivalent to WIFEXITED) */
{
	last_status = (status >> 8) & 0xff; /* Extract exit code (equivalent to WEXITSTATUS) */
}
else if (((status & 0x7f) + 1) >> 1 > 0) /* Process terminated by signal (equivalent to WIFSIGNALED) */
{
	last_status = 128 + (status & 0x7f); /* 128 + signal number (equivalent to WTERMSIG) */
}

== Syntax error exit code (< or | alone) =======
When Bash encounters a syntax error, it returns an exit code based on the formula:
256+(error code)
In many cases (including < or | alone), syntax errors return 2, meaning:
256+2=258

Bash is internally using error code 2 for syntax errors, and because exit 
statuses are stored in an 8-bit integer (0-255), they wrap around modulo 256, 
leading to 258 being displayed. The value 258 is derived from Bash’s internal handling of syntax errors, 
not from a child process that has exited with 2.