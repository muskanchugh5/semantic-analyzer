semantic-analyzer
=================

Project team members:
1. Prasoon Telang
2. Sujaya A. Maiyya
3. Vivek Agarwal

ABSTRACT


One of the front end phases of a compiler is Semantic Analysis. Parsing only verifies that the program consists of tokens arranged in a syntactically valid combination. Here we delve even deeper to check whether they form a sensible set of instructions in the programming language. This phase performs semantic checks such as type checking (checking for type errors), or object binding (associating variable and function references with their definitions), or definite assignment (requiring all local variables to be initialized before use), rejecting incorrect programs, etc. Semantic analysis usually requires a complete parse tree. 
  
Our project does the semantic analysis of Calculator Grammar i.e.,

E -> TP
P -> +TP|-TP|ϵ
T -> FQ
Q -> *FQ|/FQ|ϵ
F -> id
F -> (E) 


For any given, valid, input this program generates a parse tree. Then using a DFS traversal, it traverses through all the nodes and displays the semantic rule associated with each non-terminal/node as the output. The generated parse tree takes care of the most widely used precedence and associativity rules. The compiler need not know the precedence of each operator if it is following the grammar particularly defined for calculator.


INTRODUCTION

The translation of languages are guided by context-free grammars. We associate information with a programming language construct by attaching attributes to the grammar symbols representing the construct. Values of attributes are computed by semantic rules associated with the grammar production.

There are two notations for associating semantic rules with production, syntax-directed definitions and translation schemes. Conceptually, with both syntax-directed definitions and translation schemes, we parse the input token stream, build the parse tree, and then traverse the tree needed to evaluate the semantic rules at the parse-tree nodes.

      input  ------------->  parse  ------------->  dependency  ------------->  evaluation order
      string                       tree                            graph                            for semantic rules

A syntax-directed definition is a generalization of a context-free grammar in which each grammar symbol has an associated set of attributes, partitioned into two subsets called the synthesized and inherited  attributes of that grammar symbol.

An attribute can represent anything we choose: a string, a number, a type, a memory location, etc. The value of an attribute at a parse-tree node is defined by a semantic rule associated with the production used at that node.

The value of a synthesized attribute at a node is computed from the values of attributes at the children of that node in the parse tree; the value of an inherited attribute is computed from the values of attributes at the siblings and parent of that node.

Semantic rules set up dependencies between attributes that will be represented by a graph. From the dependency graph, we derive an evaluation order for the semantic rules. Evaluation of the semantic rules defines the values of the attributes at the node in the parse tree for the input string.

A parse tree showing the values of attributes at each node is called an annotated parse tree. The process of computing the attribute values at the nodes is called annotating the parse tree.



REQUIREMENT SPECIFICATION

The project was developed using the following:

1) TOOLS:
     - gedit
     - Terminal

2) OPERATING SYSTEM:
    - Ubuntu 12.04 (32 bit)

3) LANGUAGE:
    - C (C99)


To successfully run the project, the user requires:

1.) TOOLS:
      -  Code-blocks IDE (any version), TurboC (Windows users)
      -  cc/gcc package (Linux users)

2.) OPERATING SYSTEM:
      -  Linux (32/64 - bit, 32 bit preferred)
      -  Windows (XP, 7, Vista, 98)



IMPLEMENTATION

The project is based on receiving mathematical expression entered by the user. The result is the generation of a parse tree associated with the input by using the unambiguous context-free grammar defined for handling expressions. It also displays the semantic rule that’s needed to be applied at every node of the parse tree.

The grammar which is defined for dealing with expressions is:

E -> TP
P -> +TP|-TP|ϵ
T -> FQ
Q -> *FQ|/FQ|ϵ
F -> id
F -> (E)

Generation of Parse Tree:
The program reads the entered expression character by character and dynamically allocates the respective nodes needed as per the grammar. Dynamic allocation is done using malloc. By the time the program reads the last character, an associated parse tree will be completed using runtime memory allocation.

Eg: For T -> FQ, the program will allocate memory for node F and node Q. The program then links the lchild of T to F and rchild of T to Q.

Application of Semantic Rule:
The tree then itself undergoes DFS traversal, starting from root node,  searching for the bottom-most and unvisited node. When the program has travelled to the bottom-most node, it displays the semantic rule that’s needed to be applied in that node and then the control moves up to it’s immediate parent. This happens recursively until the control reaches back to the root node.

Clearance of dynamic storage:
Since C does not automatically return the dynamically allocated memory back to the OS, it is the job of the programmer to ensure that he frees all the memory used. The program performs the same dfs traversal to clear up the memory used by the nodes.


CONCLUSION

Challenges faced:
1. Implementing context-free grammar in C language.
2. Generation of parse tree using the mentioned grammar.
3. Dealing with F -> (E) was difficult to implement iteratively. It was made convenient by using recursion.
4. Displaying the correct associated semantic rule for each node encountered during DFS traversal.

We have learnt a lot in the journey of developing this project. This project has improved our confidence regarding coding aspect.

This project is a segment of an actual compiler. The program displays how a compiler functions when it has to solve mathematical expression. Using the mentioned grammar and semantic rules, the compiler is able to generate parse trees and achieve operator precedence to evaluate mathematical expressions involving multiplication ‘*’, division ‘/’, addition ‘+’ and subtraction ‘-’ along with open bracket ‘(‘ and close bracket ‘)’.
