# Cheaky OS
Welcome to Cheaky OS - the cheekiest operating system in town! 🎩✨

Cheaky OS is a simple skeleton OS designed to be lightweight and efficient, perfect for systems with low memory and storage. It's your go-to solution for getting things done without bogging down your hardware.

## Features
### 🧹 Scripts:

- clean.sh: Feeling cluttered? Run this script to tidy up and remove all build files, leaving your repo squeaky clean.
- build.sh: Compile everything and link them using the linker to get your OS up and running.
- ios.sh: Need an ISO file? This script has got you covered. It compiles the kernel and creates a neat isodir folder with all the essentials.
- qemu.sh: Launch your OS with style using this script. It compiles the kernel and fires up the qemu system to run the ISO file. Ready, set, go!

## 📝 Todo
- [x] Formatted Printing
- [x] String Functions
- [ ] Stack Smashing Protector
- [ ] Memory Management
- [ ] GDT
- [ ] Interrupts
- [ ] Keyboard Support
- [ ] FileSystem

And that's just the beginning! More tasks will be added once these are tackled. Are you up for the challenge?

## Setup
### ⚙️ Getting Started:

- Requirements: Ensure you have the necessary tools and dependencies installed. Check out the requirements.md file for details.
- Installation: Clone the repository to your local machine and run the provided setup scripts to get everything up and running.
- Configuration: Customize your OS experience by tweaking the configuration files to your liking. The possibilities are endless!

## Usage
### 🚀 Running Cheaky OS:

- Building: Use the build.sh script to compile the OS and prepare it for launch.
- Launching: Fire up your OS with flair using the qemu.sh script. Sit back, relax, and watch as Cheaky OS dazzles you with its charm.

### 🔧 Customization:
- Want to add your own features or tweak existing ones? Dive into the code and let your creativity flow! Don't forget to share your improvements with the community.

## Contribution
### Fork this repository

Fork this repository by clicking on the fork button on the top of this page.
This will create a copy of this repository in your account.

### Clone the repository

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

### Create a branch

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

### Make necessary changes and commit those changes

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

### Push changes to GitHub

Push your changes using the command `git push`:

```bash
git push -u origin your-branch-name
```
replacing `your-branch-name` with the name of the branch you created earlier.

Now submit the pull request.
Soon I'll be merging all your changes into the main branch of this project. You will get a notification email once the changes have been merged.

