
## Build and run locally

```
mkdir cmake-build-xxxx && cd cmake-build-xxxx
```

```
conan lock create --profile:host=../.conan/profiles/cpp20 --profile:build=default --lockfile=../lockfile.json --lockfile-out=lockfile.json --name=composite --version=0.1 ../conanfile.txt --build --update
```

```
conan install --lockfile=lockfile.json ../conanfile.txt --build=missing --generator=virtualenv
```

## Update dependencies

```
conan lock create --name=composite --version=0.1 --base --lockfile-out lockfile.json conanfile.txt --build --profile:host=.conan/profiles/cpp20 --profile:build=default
```
