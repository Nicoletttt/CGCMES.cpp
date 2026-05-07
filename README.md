# CGCMES.cpp

Collaborative Group Contribution Monitoring and Evaluation System
---------------------------------------------------------------------------------------------------------------
Problem

Group projects and activities aren't always fair. Group leaders often had a hard time manually recording the groupmates' tasks and their progress status may be difficult. They may forget to track important notes, reminders, and points of discussion for the members, which may lead to being unable to ask for a follow-up or give feedback to the other members. Giving a fair evaluation per member can be difficult for instructors or group leaders because there's no way to know how much they have actually contributed.
---------------------------------------------------------------------------------------------------------------
What is Collaborative Group Contribution Monitoring and Evaluation System?

A C++ console-based program that allows users to create and manage group members, including assigned tasks and their statuses; create important notes and reminders; and view contribution metrics and each member's rank based on the number of tasks and their progress status. 
---------------------------------------------------------------------------------------------------------------
What C++ data structures are used in this program?

The program uses vectors (dynamic arrays) to store member, task, and note lists, and structures (struct Member, struct Task, and struct Note) to organize member, task, and note data. All of the data input by the user is handled through file handling (.txt files through fstream) to save and retrieve records of members, tasks, and notes across program runs.
---------------------------------------------------------------------------------------------------------------
