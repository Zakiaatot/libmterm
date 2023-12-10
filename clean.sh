#!/bin/sh
echo "清空 /build && /ouput 目录"
rm -rf build/*
rm -rf output/*

for project_dir in example/*; do
    if [ -d "$project_dir" ]; then
        # 清空每个项目的 build 目录内容
        build_dir="$project_dir/build"
        if [ -d "$build_dir" ]; then
            echo "清空 $build_dir 目录"
            rm -rf "$build_dir"/*
        fi
    fi
done
