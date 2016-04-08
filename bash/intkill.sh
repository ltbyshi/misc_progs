#! /bin/bash

trap 'kill $$' SIGINT
$@

