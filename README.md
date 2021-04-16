# Rudimentary-command-line-interpreter
Implementation of a rudimentary command-line interpreter running on the Linux operating system

A shell	that runs	as	an	application	program	on	top	of	the	Linux	kernel.	The	shell	accepts	user	commands	(one	line	at	a	time),	and	execute	the	same.	The	
following	features	are	implemented:

a) Run	an	external	command: The	external	commands	refer	to	
executables	that	are	stored	as	files.	They	have	to	be	executed	by	spawning	a	child	
process	and	invoking	execlp() or	some	similar	system	calls.	<br>
Example	user commands:

```		./a.out	myprog.c
		cc	–o	myprog	myprog.c
		ls	-l

```
b) Run	an	external	command	by	redirecting	standard	input	from	a	file:	
The	symbol	“<”	is	used	for	input	redirection,	where	the	input	will	be	read	from	
the	specified	file	and	not	from	the	keyboard.	You	need	to	use	a	system	call	like	
“dup()”	or	“dup2()”	to	carry	out	the	redirection.	
```
Example	user	command:
		./a.out	<	infile.txt
		sort	<	somefile.txt

```
c) Run	an	external	command	by	redirecting	standard	output	to	a	file:	
The	symbol	“>”	is	used	for	output	redirection,	where	the	output	will	be	written	
to	the	specified	file	and	not	to	the	screen.	You	need	to	use	a	system	call	like	
“dup()”	or	“dup2()”	to	carry	out	the	redirection.	
Example	user	commands:
		./a.out	>	outfile.txt
		ls	>	abc

d) Combination	of	input	and	output	redirection: Here	we	use	both	“<”	
and	“>”	to	specify	both	types	of	redirection.	Example	user	command:
		./a.out	<	infile.txt	>	outfile.txt

e) Run	an	external	command	in	the	background	with	possible	input	
and	output	redirections:	We	use	the	symbol	“&”	to	specify	running	in	the	
background.	The	shell	prompt	will	appear	and	the	next	command	can	be	typed	
while	the	command	is	being	executed	in	the	background.	
Example	user commands:
    ./a.out	&
		./myprog	<	in.txt	>	out.txt	&

f) Run	several	external	commands	in	the	pipe	mode:	The	symbol	“|”	is	
used	to	indicate	pipe	mode	of	execution.	Here,	the	standard	output	of	one	
commands	will	be	redirected	to	the	standard	input	of	the	next	command,	in	
sequence.	You	need	to	use	the	“pipe()”	system	call	to	implement	this	feature.
Example	user	commands:
		ls	|	more
		cat	abc.c	|	sort	|	more
