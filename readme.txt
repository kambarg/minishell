===
To check for memory leaks run:
make leaks

===
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
