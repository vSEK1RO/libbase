# libbasen

c++ encoding/decoding from arbitrary base

<a href="https://repology.org/project/libbasen/versions">
    <img src="https://repology.org/badge/tiny-repos/libbasen.svg" alt="Packaging status">
</a>
<a href="https://repology.org/project/libbasen/versions">
    <img src="https://repology.org/badge/latest-versions/libbasen.svg" alt="latest packaged version(s)">
</a>

---

## Contents
- [Installation](#installation)
- [Contributing](#contributing)

---

## Installation

Automatically from specified repositories:

[![Packaging status](https://repology.org/badge/vertical-allrepos/libbasen.svg)](https://repology.org/project/libbasen/versions)

Manually using make:
```
make -j $(nproc)
sudo make i USRDIR=(Your installation dir)
```
Uninstall:
```
sudo make uni USRDIR=(Your installation dir)
```
For build with with debug flags:
```
make -j $(nproc) DEBUG=
```
For build tests (needed gtest package as dependency):
```
make tests -j $(nproc) DEBUG=
```
And also flag for dynamic linking (if possible):
```
make tools -j $(nproc) SHARED=
```

---

## Contributing

Main branch is under protection rules, so you should create pull request. After merging will be runned [authors.sh](https://github.com/vSEK1RO/libbasen/blob/main/authors.sh) for adding all contributors usernames and their number of commits to [AUTHORS](https://github.com/vSEK1RO/libbasen/blob/main/AUTHORS) file.

Now we would like to implement the following features:
- Base32
- BCH
- Bech32

---

[⬆️ Contents](#contents)
