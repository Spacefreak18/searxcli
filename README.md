# SEARXCLI
Perform image searches through searx server from the terminal

## Basic Usage
searxcli -s example.com "search query"
fim $(searxcli "search query")

## Dependencies
libcurl
ssl/crypto
libconfig
argtable2
json-c

## Compilation
See install.sh

## Config File
searxcli uses xdg paths. A config file can be placed in $HOME/.config/searxcli/searxcli.config
searxcli uses a libconfig formatted config file
server_url sets the url of the searx instance
max_results sets the maximum amount of individual results returned from server for search queries
max_cache sets the maximum amount of searches whose results are stored in the cache located at $HOME/.cache/searxcli
A sample config file is provided

## Command Line Options
--nocache - disables cache for current search
-c --max_cache - sets max cache effective for only current search
-r --max_results - sets the max results to return for the current search
-s --server - sets the search server to use for the current search
One unnamed argument is allowed and that is the search query

## Todo
Add file searches
