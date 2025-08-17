NOTE: This documentation refers to v1 folder. For basic understanding of `fork` and `execvp`, refer to v0. 

# How this Custom Shell Works

This project is basically a **mini shell** — like a tiny version of `bash` or `zsh`.
A shell is nothing but a program that takes commands from the user, runs them, and shows the result.

Let’s go step by step:

---

### 1. The Loop (The Heart of the Shell)

The shell keeps running inside a **while loop**.

* It shows a prompt (`$ `).
* It waits for you to type something.
* It tries to understand what you typed.
* Then it runs it.
* And then it goes back to showing the prompt again.

This repeats forever, until you type `exit`.

---

### 2. Reading What You Type

We use `fgets()` to read the command you type. For example, if you type:

```
ls -l
```

`fgets` will grab that whole line as a string.

But `fgets` also captures the `Enter` key (the newline `\n`).
So, before doing anything, we **remove the newline**. Otherwise, the command would look like `"ls\n"` instead of `"ls"`, and Linux wouldn’t understand it.

---

### 3. Splitting the Command (Tokenizing)

When you type a command, it can have multiple parts. For example:

```
ls -l /home
```

This actually has 3 parts:

1. `ls` (the program to run)
2. `-l` (an option)
3. `/home` (a folder name)

To handle this, we split the input string by spaces.
So, `"ls -l /home"` becomes an array:

```
args[0] = "ls"
args[1] = "-l"
args[2] = "/home"
args[3] = NULL
```

This is exactly the format that Linux system calls expect.

---

### 4. Forking (Creating a Child)

The shell itself cannot directly become `ls`. If it does, then the shell would be gone after one command.

So instead, we create a **child process** using `fork()`.

* The parent (our shell) stays alive.
* The child is a copy of the parent, but inside the child we immediately replace it with the real program you wanted (`ls`, `cat`, etc.) using `execvp()`.

So the child turns into the program you typed, while the parent waits.

---

### 5. Waiting for the Child

After the child starts running, the parent shell uses `waitpid()` to pause until the child is done.

This is why when you type `ls`, you see the output first, and only after that you get back the `$ ` prompt.

---

### 6. Exiting

If you type `exit`, the shell just breaks out of the loop and ends the program.

This way you can close the shell gracefully.

---

### 7. Memory Safety

* Every time we read a command, we allocate memory (`malloc`).
* After using it, we free it so the program doesn’t waste memory.
* We also handle resizing of the arguments array in case the command has many parts.

---

# Summary

My shell is like a middleman:

* It **listens** to what you type.
* It **splits** it into understandable parts.
* It **creates a child** process to run the program.
* It **waits** for the child to finish.
* Then it comes back and asks you again: *“What next?”*

That’s literally all a shell does — but now I have my own working version of it 🎉
