#!/usr/bin/env bash
set -e
output_dir="$(make -s print-OUTPUT_DIR)"
suffix="$(make -s print-BIN_SUFFIX)"
modules="$(make -s print-MODULES_DIR)"
lib="$(make -s print-LIB_DIR)"
os="$(make -s print-OS)"
target=$(basename "$PWD")
make_flags="TARGET_NAME=\"$target\""

export PATH=$(find $lib/$os/ -maxdepth 1 -type d | tr '\n' ':'):$PATH

make_all() {
    for dir in $modules/*; do
        if [ -f "$dir/Makefile" ]; then
            make -C "$dir"
        fi
    done
}

# 简单帮助
usage() {
    cat << EOF
Usage: $(basename "$0") <command> [args...]
    Commands:
    run                 Build (if needed) and run the target.
                        eg: 
                            ./build.sh run <target> [args...]
                                [args...] : arguments to pass to the emulator
    
    all                 Build (if needed) and run the main.
                        eg: 
                            ./build.sh all
    
    static              Build (if needed) and make static lib.
                        eg: 
                            ./build.sh static
    
    dynamic             Build (if needed) and make dynamic lib.
                        eg: 
                            ./build.sh dynamic
    
    test                Build test.
                        eg: 
                            ./build.sh test <test_name> [args...]
                                <test_name> : name of the %.c (without suffix) in the test dir 
                                [args...] : arguments to pass to the test
    test-group          Build and run grouped single tests.
                        eg:
                            ./build.sh test-group <group>
                                <group> : core | rule | ui | game | single | all
    clean               Clean the project.
    help                Show this help.
EOF
}

run_test_target()
{
    test_target="$1"
    flag="$output_dir/$test_target$suffix"

    make_all
    make "$flag" $make_flags
    check_and_run "$flag"
}

list_group_targets()
{
    group_name="$1"

    case "$group_name" in
        core)
            echo "single/test_card single/test_core single/test_event single/test_player"
            ;;
        rule)
            echo "single/test_rule"
            ;;
        ui)
            echo "single/test_ui single/test_ui_hint"
            ;;
        game)
            echo "single/test_game single/test_game_flow single/test_game_lifecycle"
            ;;
        single|all)
            find test/single -type f -name "*.c" | sed 's#^test/##' | sed 's#\.c$##' | sort
            ;;
        *)
            return 1
            ;;
    esac
}

check_and_run()
{
    exe="$1"; shift
    args="$*"
    if [ -x "$exe" ]; then
        $exe $args
    else
        echo "Error: executable $exe not found."
        exit 1
    fi
}

cmd="$1"; shift || true
case "$cmd" in
    run)
        if [ -z "$1" ]; then
            make_all
            make all "$make_flags"
        else
            flag="$output_dir/$1$suffix"
            make_all
            make $flag "$make_flags"
            shift
            args="$*"
            make echo-version
            check_and_run "$flag" $args
        fi
        ;;
    static)
        echo "--------------------------------"
        echo "make static"
        echo "--------------------------------"
        make static
        ;;
    dynamic)
        echo "--------------------------------"
        echo "make dynamic"
        echo "--------------------------------"
        make dynamic
        ;;
    debug)
        # TODO: implement debug build
        echo "Debug build not implemented yet."
        ;;
    build)
        if [ -z "$1" ]; then
            make_all
            make all $make_flags
        else
            flag="$output_dir/$1$suffix"
            make_all
            make $flag $make_flags
        fi
        ;;
    test-group)
        if [ -z "$1" ]; then
            echo "Error: missing group name"
            usage
            exit 2
        fi

        group="$1"
        targets="$(list_group_targets "$group")" || {
            echo "Error: unknown test group '$group'"
            echo "Available groups: core rule ui game single all"
            exit 2
        }

        for t in $targets; do
            run_test_target "$t"
        done
        ;;
    clean)
        make clean
        ;;
    help|--help|-h|"")
        usage
        ;;
    my)
        clear
        make -C src/platform
        ./build.sh dynamic
        ./build.sh run main
        ;;
    *)
        echo "Unknown command: $cmd"
        usage
        exit 2
        ;;
esac
