# How to contribute to this project

## Git-Flow

We use the git-flow pattern as described [here](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow).

The **master** branch contains only stable releases.

The **develop** branch is the base for all new features implementation and bug fixes.

We track issues with GitHub issues module, and we use GitHub project to organize issues and plan new developments,

When a developer starts wokring on an open issue, he should start a new feature branch, with a name related to the issue and if possible to issue ref.

> for example, a goog pattern name could be: ```/features/<name of feature>```  or ```/features/#ref_name of the feature``` 

When a feature is marked as completed, it needs to be tested by one of the project maintainers before to be merged in develop.

When a number of feature are closed and we consider tested and stable the currente develop branch, we merge into master and create a new release.


