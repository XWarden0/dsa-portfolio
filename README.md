# dsa-portfolio
Portfolio for CS-300
##REFLECTION
**What problem was I solving in these projects?**
I built an advising assistance tool for ABCU that needs to (1) print an alphanumeric list of all Computer Science courses and (2) given a course number, print the course title and its prerequisites. The broader goal was to design and implement these features using appropriate data structures and to justify the choice with runtime and memory analysis.
**How did you approach the problem? Consider why data structures are important to understand**
I first wrote psuedocode for three structures--vector, hash table, and binary seart tree (BST)--so I could reason about their behavior before coding. Then I analyzed the runtime tradeoffs: vectors are simple but require sorting and linear lookup; hash tables give O(1) average lookups but still need a key sort to print; BSTs give natural in-order traversal but can degrade without balancing. Understanding these characteristics let me pick a design that meets both features efficiently.
**How did you overcome any roadblocks you encountered while going through the activities project?**
I ran into typical issues: CSV parsing quirks, making lookups case-insensitive, ensuring every prerequisite actually exists in the file, and avoiding duplicate course IDs. I solved these with a two-pass load, consistent whitespace trimming and uppercase normalization for course IDs, clear warnings/errors during load, and small, focused functions so bugs were easier to isolate. I also verified build/run paths in my IDE to avoid "file not found" surprises.
**How has your work on this project expanded your approach to designing software and developing programs?**
I leaned into separation of concerns: one part reads and validates input, another stores data, and another prints results. That mindset made it straightforward to swap data structures or adjust output formatting. I also got more deliberate about measure cost--designing with Big-O in mind instead of guessing.
**How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?**
I used descriptive names, concise functions, inline comments where intent wasn't obvious, and consistent input normalization. I added friendly error messages and avoided hidden assumptions. As a result, the code is easier to test, and adapting it--for example, switching from a hash table to a balanced BST--would require minimal changes.
