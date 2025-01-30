# Laying the Foundation: The Power of Good Code Design

When building a project as complex as a shell, good code design is not just helpful—it’s essential. Without a solid foundation, maintaining and expanding your project can quickly become a nightmare. In this section, we’ll explore key design principles that made the development of Minishell smoother, more maintainable, and less error-prone.

## The Importance of Code Structure

Minishell, like any other program, needs to be readable and organized. A well-structured project allows you to debug issues more easily, add new features without breaking existing functionality, and collaborate effectively with others. Two of the most crucial principles we adhered to in our design were:

### 1. Single Responsibility Principle (SRP)

Each function should have only one job. This might sound simple, but it’s one of the most overlooked principles in software design. When a function is responsible for multiple tasks, it becomes harder to test, debug, and reuse.

#### Example: Code Without SRP

```c
void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        // Process text
        for (int i = 0; buffer[i]; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                buffer[i] -= 32; // Convert to uppercase
            }
        }
        
        // Save output
        FILE *out_file = fopen("output.txt", "a");
        if (out_file) {
            fputs(buffer, out_file);
            fclose(out_file);
        }
    }
    fclose(file);
}
```

In this example, the function is doing too much:

- It reads the file.
- It processes the text.
- It writes the output.

This makes it difficult to modify any single part without affecting the rest.

#### Example: Code Using SRP

```c
void read_file(const char *filename, void (*process_line)(char *)) {
    FILE *file = fopen(filename, "r");
    if (!file) return;
    
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        process_line(buffer);
    }
    fclose(file);
}

void convert_to_uppercase(char *buffer) {
    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            buffer[i] -= 32;
        }
    }
}

void save_to_file(const char *filename, char *content) {
    FILE *file = fopen(filename, "a");
    if (file) {
        fputs(content, file);
        fclose(file);
    }
}

int main() {
    read_file("input.txt", convert_to_uppercase);
    save_to_file("output.txt", "Processed text\n");
    return 0;
}
```

Now, each function has a single responsibility:

- `read_file` handles file reading.
- `convert_to_uppercase` processes the text.
- `save_to_file` handles writing output.

This approach makes it easier to maintain and test each function independently.

### 2. Single Source of Truth (SSOT)

Bugs often occur when multiple parts of a program store the same data in different places. To avoid this, we establish a single source of truth for critical information. Instead of duplicating data across different modules, we centralize it in one place, ensuring consistency and reducing errors.

In a shell implementation like Minishell, lexemes play a crucial role in parsing and interpreting commands. If different parts of the program define or manipulate lexemes independently, inconsistencies can arise, making debugging difficult and leading to unexpected behavior. By centralizing lexeme definitions in a single module, we ensure that all components of the shell reference the same structure, avoiding duplication and maintaining coherence.

For example, having a dedicated lexer module that defines and processes lexemes ensures that any change to tokenization logic is applied consistently across the entire project. Without this centralization, one part of the program might classify a string differently than another, leading to parsing errors and inconsistent command execution.

## The Benefits of Good Code Design

By following SRP and SSOT, we achieved:

- **Easier debugging**: When something breaks, we know exactly where to look.
- **Better scalability**: Adding new features (like piping or job control) becomes much easier.
- **Improved readability**: Future contributors (or even your future self) can quickly understand the code.

With a well-structured design in place, we were ready to dive into the actual implementation of Minishell. Next, we’ll explore the project requirements and what was expected from us as developers.

## Further Reading

If you're interested in diving deeper into software design principles, here are some key concepts worth exploring:

- **Design Patterns**: Common solutions to recurring software design problems, such as Factory Pattern, Singleton Pattern, and Observer Pattern.
- **Behavioral Design Patterns**: Patterns that focus on communication between objects, such as Strategy Pattern, Command Pattern, and Chain of Responsibility.

Understanding and applying these concepts can further enhance your ability to write clean, maintainable, and scalable code.

---

# Understanding the Minishell Project

Before diving into the implementation details, it is crucial to understand what the Minishell project entails. This section will outline the project’s requirements and what is expected from a developer tackling this challenge.

## Project Overview

Minishell is a simplified shell that mimics the behavior of Bash but with limited functionalities. The goal is to implement a command-line interpreter capable of executing basic shell commands while handling input, output, and built-in functions. It serves as an excellent introduction to system programming and process management.

### What Is Expected?

As a developer working on Minishell, you are expected to:

- Display a prompt when waiting for a new command.
- Have a working history.
- Search and launch the right executable (based on the PATH variable or using a
  relative or an absolute path).
- Not use more than one global variable. Think about it. You will have to explain
  its purpose.
- Not interpret unclosed quotes or special characters which are not required by the
  subject such as \ (backslash) or ; (semicolon).
- Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
- Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).

- Implement redirections: 
  - `<` should redirect input.
  - `>` should redirect output. 
  - `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
  - `>>` should redirect output in append mode.
- Implement pipes (| character). The output of each command in the pipeline is
  connected to the input of the next command via a pipe.
- Handle environment variables ($ followed by a sequence of characters) which
  should expand to their values.
- Handle $? which should expand to the exit status of the most recently executed
  foreground pipeline.
- Handle Signals : `Ctrl+C`, `Ctrl+D` and `Ctrl+\` which should behave like in bash.
- Your shell must implement the following builtins:
  - `echo` with option -n
  - `cd` with only a relative or absolute path
  - `pwd` with no options
  - `export` with no options
  - `unset` with no options
  - `env` with no options or arguments
  - `exit` with no options
- Bonus part:
  - `&&` and `||` with parenthesis for priorities.
  -   Wildcard `*` should work for the current working directory

This project isn’t just about making a functional shell—it’s about building one that is well-structured, efficient, and easy to maintain. Knowing the goals from the start helps in designing a solid foundation before coding. In this blog, we’ll walk through the steps we took to complete the project, including the bonus part. Here are the steps:

1. **Understanding Shell Grammar**
2. **Parsing user input and building the AST**
3. **Execution: AST traversal and redirections handling**
4. **Environment Variable and Wildcard expansions**.
5. **Builtin commands**.
6. **Signal handling**.
7. **Error management.**

## 1. Understanding Shell grammar

Source: **IEEE Std 1003.1**, [opengroup.org - Shell Command Language](https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html)

​	Imagine you have a big box of LEGO bricks. You can use them to build anything—a house, a car, or even a spaceship! But if you just stick random bricks together without a plan, your creation might fall apart or not look like anything at all. Grammar is like the instructions for building with LEGOs. It tells you how to put the pieces (words) together in the right way so that your creation (a sentence) makes sense. If you follow the rules, you can build something strong and clear. But if you don’t, your words might get jumbled, and people won’t understand what you’re trying to say!

Just like how LEGO instructions guide you in building something sturdy, grammar rules define how shell commands should be structured. A parser acts like a LEGO master builder—it checks if the pieces (words and symbols) are put together correctly according to the instructions. If you randomly connect LEGO bricks without following the rules, your structure might collapse. Similarly, if a command doesn’t follow the correct syntax, the parser should catch the mistake and prevent the shell from executing something invalid. 

A well-designed parser doesn’t just hardcode specific mistakes; instead, it understands the deeper rules of how commands should be formed. This way, it can detect errors naturally, rather than relying on checking for every possible mistake separately.

Understanding grammar is essential for building the parser. Here is a simplified version of Shell grammar that aligns with the Minishell project requirements:

``` 
complete_command ; linebreak command_list linebreak
                 ;
command_list     : compound_command linebreak command_list
			    | compound_command
                 ;
compound_command : pipeline '&&' linebreak compound_command
                 | pipeline '||' linebreak compound_command
			    | pipeline
                 ;
pipeline         : command '|' linebreak pipeline
                 | command
                 ;
command          : subshell
			    | simple_command
                 ;
subshell         : '(' command_list ')' redirect_list*
                 ;
simple_command   : io_redirect simple_command
			    | io_redirect
                 | WORD        simple_command
                 | WORD
                 ;
redirect_list    : io_redirect redirect_list*
                 ;
io_redirect      : '<'     WORD(filename) 
                 | '>'     WORD(filename)
                 | '>>'    WORD(filename)
                 | '<<'    WORD 		
			    ;
linebreak        : NEWLINE* linebreak*
```

Here’s a step-by-step breakdown of the above mentioned grammar, starting from the **complete_command** down to the **WORD** token:  

1. **complete_command**:  
   - This represents a full shell command.  
   - It consists of a **command_list** surrounded by optional **linebreaks** (which handle newlines and formatting).  
   - The second rule (`;`) allows for an empty command, meaning the shell can handle blank lines without errors.  

2. **command_list**:  
   - A list of commands that need to be executed.  
   - It consists of at least one **compound_command**, followed by an optional additional **command_list** (allowing for multiple commands to be chained together).  

3. **compound_command**:  
   - This handles command execution logic, particularly logical operators:  
     - `&&`: Execute the next command only if the previous one succeeds.  
     - `||`: Execute the next command only if the previous one fails.  
   - A command can be a **pipeline** (explained next), optionally followed by `&&` or `||`, allowing for conditional execution chains.  

4. **pipeline**:  
   - Defines how commands are linked using pipes (`|`).  
   - A **command** can be followed by `|` to pass its output to the next command in the pipeline.  
   - If no pipe is present, it’s just a single command.  

5. **command**:  
   - A **command** can either be:  
     - A **subshell** (a group of commands inside parentheses that execute in a separate environment).  
     - A **simple_command** (a basic shell command with optional arguments or redirections).  

6. **subshell**:  
   - A command group enclosed in parentheses `()`.  
   - This allows commands to be executed in a separate process.  
   - A **redirect_list** may optionally follow, meaning input/output redirection applies to the entire subshell.  

7. **simple_command**:  
   - The most basic command structure.  
   - It consists of:  
     - An optional **io_redirect** (handling input/output redirection).  
     - One or more **WORD** tokens (representing the command name and arguments).  

8. **redirect_list**:  
   - A sequence of **io_redirect** rules.  
   - Allows multiple redirections to be attached to a command (e.g., `command > file 2>&1`).  

9. **io_redirect**:  
   - Specifies how input and output are handled.  
   - Examples:  
     - `< file` (read input from a file).  
     - `> file` (write output to a file, overwriting).  
     - `>> file` (append output to a file).  
     - `<< WORD` (here-document, where input is provided inline).  

10. **linebreak**:  
    - Handles newlines (`NEWLINE*`).  
    - Ensures that multiple blank lines don’t break the parsing process.  

11. **WORD**:  
    - The most fundamental unit.  
    - Represents command names, arguments, filenames, or here-document delimiters.  

### **Why This Structure Matters**  

- This grammar is **hierarchical**, meaning larger structures are built from smaller components.  
- The use of recursion (e.g., `command_list → compound_command → pipeline → command`) allows the parser to handle complex commands without manually listing all edge cases.  
- Instead of hardcoding syntax checks, this structured approach naturally enforces correct shell command syntax while allowing flexibility.
