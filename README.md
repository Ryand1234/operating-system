# Cheaky OS
A simple skeleton os which is light weight and efficient to do task on systems with low memory and storage.

## Scripts:
- clean.sh: It will remove all the build files making the repo clean. It will remove all the files such as .o, .d, sysroot/ etc
- build.sh: Build script to compile everything to create object files and link them using linker
- ios.sh: It calls build script internally to first create object files and then create a isodir folder containing compiled kernal and create .iso file.
- qemu.sh: It calls iso script internally to compile kernel and then it calls the qemu system to run the .iso file.

# Contribution
## Fork this repository

Fork this repository by clicking on the fork button on the top of this page.
This will create a copy of this repository in your account.

## Clone the repository

Now clone the forked repository to your machine. Go to your GitHub account, open the forked repository, click on the code button and then click the _copy to clipboard_ icon.

Open a terminal and run the following git command:

```bash
git clone "url you just copied"
```

where "url you just copied" (without the quotation marks) is the url to this repository (your fork of this project). See the previous steps to obtain the url.


For example:

```bash
git clone git@github.com:Ryand1234/operating-system.git
```

where `this-is-you` is your GitHub username. Here you're copying the contents of the first-contributions repository on GitHub to your computer.

## Create a branch

Change to the repository directory on your computer (if you are not already there):

```bash
cd operating-system
```

Now create a branch using the `git switch` command:

```bash
git switch -c your-new-branch-name
```

For example:

```bash
git switch -c add-alonzo-church
```

## Make necessary changes and commit those changes

Now open `Contributors.md` file in a text editor, add your name to it. Don't add it at the beginning or end of the file. Put it anywhere in between. Now, save the file.

If you go to the project directory and execute the command `git status`, you'll see there are changes.

Add those changes to the branch you just created using the `git add` command:

```bash
git add Contributors.md
```

Now commit those changes using the `git commit` command:

```bash
git commit -m "Add your-name to Contributors list"
```

replacing `your-name` with your name.

## Push changes to GitHub

Push your changes using the command `git push`:

```bash
git push -u origin your-branch-name
```
replacing `your-branch-name` with the name of the branch you created earlier.

Now submit the pull request.
Soon I'll be merging all your changes into the main branch of this project. You will get a notification email once the changes have been merged.

