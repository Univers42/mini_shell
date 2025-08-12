# Git Flow Complete Cheat Sheet 📚

## What is Git Flow? 🤔

Git Flow is like having a **roadmap for your code**! It's a branching strategy that helps teams organize their work by giving each type of task its own "lane" (branch). Think of it like organizing your school subjects - you wouldn't mix your math homework with your history essay, right?

## The Philosophy Behind Git Flow 🧠

**The Big Idea**: Keep different types of work separated so they don't interfere with each other. It's like having separate folders for:
- Your finished, published work (production)
- Your work-in-progress (development)
- Your experiments and new ideas (features)
- Your urgent fixes (hotfixes)

---

## The 5 Types of Branches 🌳

### 1. **Main Branch** (formerly master)
- **What it is**: Your "published book" - the official, stable version
- **Rule**: Only finished, tested, production-ready code goes here
- **Think of it as**: The final draft you'd hand to your teacher
- **Never**: Work directly on this branch

### 2. **Develop Branch**
- **What it is**: Your "rough draft" - where you combine everyone's work
- **Rule**: All new features get merged here first
- **Think of it as**: Your working document where you put together all your chapters
- **This is where**: The team collaborates and tests things together

### 3. **Feature Branches**
- **What it is**: Individual "chapters" or new functionality
- **Naming**: `feature/user-login` or `feature/shopping-cart`
- **Rule**: One feature = one branch
- **Think of it as**: Writing each chapter of your book separately
- **Lifespan**: Created from `develop`, merged back to `develop` when done

### 4. **Release Branches**
- **What it is**: "Proofreading phase" - preparing for publication
- **Naming**: `release/v1.2.0`
- **Rule**: No new features, only bug fixes and final touches
- **Think of it as**: Final editing before submitting your paper
- **Lifespan**: Short-lived, gets merged to both `main` and `develop`

### 5. **Hotfix Branches**
- **What it is**: "Emergency corrections" - fixing critical bugs in production
- **Naming**: `hotfix/critical-login-bug`
- **Rule**: Fix urgent problems in production immediately
- **Think of it as**: Correcting a major error after you've already submitted
- **Special**: Created from `main`, merged to both `main` and `develop`

---

## Step-by-Step Workflow 📝

### Setting Up Git Flow

```bash
# Initialize git flow in your project
git flow init

# Follow the prompts (usually accept defaults):
# - main branch: main
# - develop branch: develop
# - feature prefix: feature/
# - release prefix: release/
# - hotfix prefix: hotfix/
```

### Working with Features 🆕

#### Starting a New Feature
```bash
# Creates and switches to feature/awesome-login
git flow feature start awesome-login

# What this does:
# 1. Creates branch from develop
# 2. Switches you to the new branch
# 3. You're ready to code!
```

#### Working on Your Feature
```bash
# Code normally, make commits
git add .
git commit -m "Add login form"
git commit -m "Add password validation"

# Push to share with team (first time)
git flow feature publish awesome-login

# Or just push normally
git push origin feature/awesome-login
```

#### Finishing a Feature
```bash
# When your feature is complete
git flow feature finish awesome-login

# What this does:
# 1. Merges feature into develop
# 2. Deletes the feature branch
# 3. Switches you back to develop
# 4. Your feature is now part of the main development line!
```

### Creating a Release 🚀

#### Starting a Release
```bash
# Start preparing version 1.2.0
git flow release start 1.2.0

# What this does:
# 1. Creates release/1.2.0 from develop
# 2. Time for final testing and bug fixes
# 3. NO new features allowed!
```

#### During Release
```bash
# Fix bugs, update version numbers, write changelog
git commit -m "Fix typo in user manual"
git commit -m "Update version to 1.2.0"

# Test everything thoroughly!
```

#### Finishing a Release
```bash
git flow release finish 1.2.0

# What this does (it's like magic!):
# 1. Merges release into main
# 2. Creates a tag (v1.2.0)
# 3. Merges release back into develop
# 4. Deletes release branch
# 5. Your code is now live!
```

### Handling Hotfixes 🚨

#### Emergency! Production Bug!
```bash
# Create hotfix from main (production)
git flow hotfix start critical-bug

# Fix the urgent issue
git add .
git commit -m "Fix critical login security issue"
```

#### Finishing Hotfix
```bash
git flow hotfix finish critical-bug

# What this does:
# 1. Merges into main (fixes production immediately)
# 2. Merges into develop (keeps development up-to-date)
# 3. Creates a tag
# 4. Deletes hotfix branch
```

---

## Daily Workflow Examples 📅

### Monday Morning - Starting New Feature
```bash
git checkout develop
git pull origin develop          # Get latest changes
git flow feature start user-profile
# Start coding...
```

### Tuesday - Continuing Feature Work
```bash
git checkout feature/user-profile
# Code, code, code...
git add .
git commit -m "Add profile picture upload"
git push origin feature/user-profile
```

### Friday - Feature Complete
```bash
git flow feature finish user-profile
git push origin develop          # Share completed feature
```

### Release Day - Every 2 Weeks
```bash
git checkout develop
git pull origin develop
git flow release start 2.1.0
# Test, fix bugs, update docs...
git flow release finish 2.1.0
git push --all && git push --tags
```

---

## Best Practices 💡

### Do's ✅
- **Always start features from develop**
- **Keep feature branches small and focused** (1-2 weeks max)
- **Write descriptive branch names**: `feature/user-authentication` not `feature/stuff`
- **Test your features before finishing them**
- **Pull latest develop before starting new features**
- **Use meaningful commit messages**

### Don'ts ❌
- **Never work directly on main or develop**
- **Don't add new features during release phase**
- **Don't let feature branches live too long**
- **Don't skip testing before finishing features**
- **Don't forget to push your branches for team visibility**

---

## Common Commands Cheat Sheet 🔧

```bash
# Setup
git flow init

# Feature workflow
git flow feature start <name>      # Start new feature
git flow feature publish <name>    # Share feature with team
git flow feature finish <name>     # Complete feature

# Release workflow
git flow release start <version>   # Start release
git flow release finish <version>  # Complete release

# Hotfix workflow
git flow hotfix start <name>       # Emergency fix
git flow hotfix finish <name>      # Deploy fix

# Useful git commands
git branch -a                      # See all branches
git status                         # Check current status
git log --oneline --graph         # Visual commit history
```

---

## Troubleshooting 🔧

### "I'm on the wrong branch!"
```bash
git stash                    # Save your work
git checkout correct-branch  # Switch to right branch
git stash pop               # Get your work back
```

### "I need to update my feature with latest develop"
```bash
git checkout feature/my-feature
git merge develop           # Bring in latest changes
# Resolve any conflicts
```

### "I accidentally committed to develop"
```bash
git reset HEAD~1            # Undo last commit (keeps changes)
git stash                   # Save changes
git checkout feature/my-feature
git stash pop               # Apply changes to correct branch
```
