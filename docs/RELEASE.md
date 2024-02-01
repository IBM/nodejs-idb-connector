# Release

To create a new release the developer first needs to run `release.sh`
```sh
    ./release.sh
```

This script will create a new release branch, run release-it, push the new branch upstream.

From there the developer needs to:

1) Open a pull request, have it approved by at least 1 reviewer, and merged into the master branch
2) Create a new release and tag from the [GitHub Web UI](https://github.com/IBM/nodejs-idb-connector/releases/new) with the release and tag name matching the version number in package.json.


After the release is created our Jenkins instance will:

1) Build the package
2) Update the GH release with the binary asset
3) Publish the release to NPM

After the release, the package version should be bumped to a `*-devel` suffix.
This will make node-pre-gyp build from source in development builds or when installing the project from a git url.
