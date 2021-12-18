Wacky Racers
===============

This contains a directory structure for your assignment.

doc -- documentation

hw  -- hardware

src -- source code


Assignment instructions
-----------------------

The [instructions](https://eng-git.canterbury.ac.nz/wacky-racers/wacky-racers/-/raw/master/doc/instructions/instructions.pdf) are in a PDF file stored in this repository (you'll also get a copy when you fork the project).


Assignment guide
----------------

The [guide](https://eng-git.canterbury.ac.nz/wacky-racers/wacky-racers/-/raw/master/doc/guide/instructions.pdf) are in a PDF file stored in this repository (you'll also get a copy when you fork the project).


Downloading
-----------

Your group leader should create a forked copy of the wacky-racers
git project and then add the other group members to the project.  This
can be done by:

1. Go to https://eng-git.canterbury.ac.nz/wacky-racers/wacky-racers

2. Click 'Fork' button.  This will create your own copy of the repository.

3. Click on the ‘Settings’ menu then click the 'Expand' button for
'Sharing and permissions'.  Change 'Project Visibility' to 'Private'.

4. Click on the 'Members' menu and add group members as Developers.

5. Using bash terminal (or other useful shell), enter the command:

```
$ git clone git@eng-git.canterbury.ac.nz:your-userid/wacky-racers.git
```

Note: this *requires* that you have setup SSH keys on your machine and added
them to your account. See
https://docs.gitlab.com/ee/ssh/#generating-a-new-ssh-key-pair for instructions
on generating an SSH key pair.

Or if you don't want to set up your SSH keys, you can clone using the HTTPS
protocol (we recommend the above method as it allows automatic authentication
without entering your password every single time you want to make changes):

```
$ git clone https://eng-git.canterbury.ac.nz/your-userid/wacky-racers.git
```

6. Add the original wacky-racers repository as upstream. This will allow you to
   pull updates/bug fixes we make during the assignment.
```
$ cd wacky-racers
$ git remote add upstream https://eng-git.canterbury.ac.nz/wacky-racers/wacky-racers.git
```

7. If we add more demo code or tweak the instructions, you can pull the updated
   stuff using
```
$ git pull upstream master
```

