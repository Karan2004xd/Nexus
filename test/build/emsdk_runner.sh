#!/bin/bash

# generated logs message
print_log() {
  msg="$1"
  echo -e "\n------- ${msg} -------\n"
}

# donwloads emscripten
download_and_install_emsdk() {
  path="$1"
  if [ -d ${path} ]; then
    cd ${path}

    if [ -d "emsdk" ]; then
      print_log "emsdk is already installed in the current directory"
      exit 0
    fi
    print_log "Installing emsdk at path: $(pwd)"

    git clone https://github.com/emscripten-core/emsdk.git
    cd emsdk

    git pull

    # Download and install the latest SDK tools.
    ./emsdk install latest

    # Make the "latest" SDK "active" for the current user. (writes .emscripten file)
    ./emsdk activate latest

    # Activate PATH and other environment variables in the current terminal
    source ./emsdk_env.sh
  else
    print_log "${path} not found"
  fi
}

target_file=""    # file in which the assembly target code is generated like javascript file
header_file=""    # header files to be included
main_file=""      # main_file for which the script has to be run
root_dir=""       # root directory where the main_file is present and the code will be generated here itself
emsdk_dir=""      # main directory in which the emscripten sources are installed

env_file="emsdk_runner_env.txt"       # file where the variable for default root_dir and others are stored
env_vars=()

check_flag=false  # just a flag used by functions

# sets the path where the emsdk directory is installed
set_emsdk_dir() {
  temp_emsdk_dir=$(cd ${root_dir} && cd .. && cd build && pwd)
  temp_emsdk_dir+="/emsdk/upstream/emscripten"
  if [ -d $emsdk_dir ]; then
    emsdk_dir=${temp_emsdk_dir}
  else
    print_log "${temp_emsdk_dir} directory was not found\nRun emsdk_runner.sh -i (for installing)"
    exit 1
  fi
}

# checks whether the main_file actually exists in the root_dir
check_for_correct_binaries() {
  for i in $(ls ${root_dir}); do
    if [ $i = ${main_file} ]; then
      check_flag=true
      break
    fi
  done

  if [ $check_flag = true ]; then
    print_log "Filename found"
    print_log "Executing Emscripten"
  else
    print_log "${main_file} was not found in the directory ${root_dir}"
  fi
}

# executed the empscripten script after all the checks
generate_using_emsdk() {
  emsdk_exec="${emsdk_dir}/emcc"
  cd .. && cd ${root_dir}
  
  print_log "Genereating emscripten files in $(pwd)"
  echo "Command: ${emsdk_exec} ${header_file} ${main_file} -o ${target_file}"

  $(${emsdk_exec} ${header_file} "${root_dir}/${main_file}" -o ${target_file})
}

create_env_variables() {
  echo -e "ROOT_DIR=${root_dir}\nHEADER_FILE=${header_file}\nTARGET_FILE=${target_file}" > ${env_file}
}

set_local_variables_using_env() {
  if [ ! -f ${env_file} ]; then
    print_log "${env_file} was not found. Please run the script with -r flag to generate the file"
    exit 1
  fi

  for line in $(cat ${env_file})
  do
    env_name=$(echo ${line} | cut -d '=' -f1)
    env_value=$(echo ${line} | cut -d '=' -f2)

    if [ "$env_name" = "ROOT_DIR" ]; then
      root_dir="${env_value}"
    elif [ "$env_name" = "HEADER_FILE" ]; then
      header_file="${env_value}"
    elif [ "$env_name" = "TARGET_FILE" ]; then
      target_file="${env_value}"
    fi
  done
}

get_user_input() {
  read -p "Enter the name of the directory (where emscripten file is to be executed): " root_dir
  read -p "Enter the target file name (app.js for example): " target_file
  read -p "Enter the header_file to be entered (if needed or else leave blank): " header_file
  
  # -z header_file returns true if the string is empty
  if [ ! -z $header_file ]; then
    header_file="--${header_file}"
  fi

  # -n root_dir returns false if string is not empty
  if [ -n $root_dir ]; then
    dir_to_exec=$(cd .. && pwd)
    dir_to_exec+="/${root_dir}"
    root_dir=${dir_to_exec}
  fi
}

# main function
run_emsdk() {
  main_file="$1"

  # -d checks if the root_dir specified by the user, actually exists or not
  # returns true if it does and false otherwise
  if [ -d $root_dir ]; then
    print_log "Directory found"
    check_for_correct_binaries
    if [ $check_flag = true ]; then
      set_emsdk_dir
      generate_using_emsdk
      print_log "Emscripten executed successfully"
    fi
  else
    print_log "${root_dir} is not a directory"
  fi
}

# script for multiple_files

cpp_directory_names=()
cpp_file_names=()

build_dir=""
build_dir_name=""
root_dir_name=""

set_directories() {
  temp_root_path="$1"
  temp_build_path="$2"

  if [ -d $temp_root_path ] && [ -d $temp_build_path ]; then
    root_dir=$(cd $temp_root_path && pwd)
    build_dir=$(cd $temp_build_path && pwd)

    # set root_dir_name, build_dir_name

    print_log "Found root directory : $root_dir"
    print_log "Found build directory : $build_dir"
  else
    print_log "${temp_root_path} is not a directory"
    exit 1
  fi
}

read_directories_names() {
  cd "${root_dir}"
  for dir in $(ls); do
    if [ -d $dir ] && [ $dir != $build_dir ]; then
      echo "$dir"
      cpp_directory_names+=("$dir")
    fi
  done
}

run_emsdk_for_multiple_files() {
  set_directories "$1" "$2"
  read_directories_names
}


while getopts "i:r:d:b p " execute; do
  main_file=$(echo $OPTARG)

  case "$execute" in
    i)
      download_and_install_emsdk "$main_file"
      ;;
    r)
      get_user_input
      create_env_variables
      run_emsdk "$main_file"
      ;;
    d)
      set_local_variables_using_env
      run_emsdk "$main_file"
      ;;
    b)
      read -p "Path to the project root directory (relative or absolute): " root_path
      read -p "Path to the build directory or directory containing cmake build files (relative or absolute): " build_path
      run_emsdk_for_multiple_files "${root_path}" "${build_path}"
      ;;
    p)
      ;;
    \?)
      echo "Invalid Input"
      print_log "Allowed input flags"
      echo "-i {relative path} : for installed in the emscripten packages (for current directory enter '.')"
      echo "-r {filename} : for running the script for the single filename specified and override the previously used info"
      echo "-d {filename} : for running the single file with previously used path and headers and flags"
      ;;
  esac
done
