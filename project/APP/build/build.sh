
# 配置文件必须与build.sh脚本放在同一个目录下
COMPILE_CFG_FILE="project_compile.cfg"

#是否需要做flint: no | yes
NEED_FLINT="no"

declare -a prod_nm_a
declare -a output_a
declare -a project_a
declare -a options_a

# 加载编译配置文件
load_config() {
    if [ ! -e ${1} ]; then
        echo "load config error: ${1} is not exist"
        return 1
    fi

    idx=x
    while read line; do
        # 去除注释和空白行
        line=${line%%\#*}
        line=`echo "${line}"| sed s/^[[:space:]]*$//g`
        if [ -z "${line}" ]; then
            continue
        fi

        # 解析key-value文件
        group_nm=`echo ${line} | sed -n -e 's/\[\(.*\)\]/\1/p'`
        if [ -n "${group_nm}" ]; then
            if [ "${idx}" = "x" ]; then
                idx=0
            else
                if [ -z "${output_a[$idx]}" ]; then
                    echo "\e[1;31m[${prod_nm_a[${idx}]}] output_path is invalid"
                    return 1
                fi

                if [ -z "${project_a[$idx]}" ]; then
                    echo "\e[1;31m[${prod_nm_a[${idx}]}] project_file is invalid"
                    return 1
                fi
                idx=$((idx+1))
            fi
            prod_nm_a[${idx}]=${group_nm}
        else
            if [ "${idx}" = "x" ]; then
                continue
            fi
            key=`echo ${line} | awk -F= '{print $1}' | sed -n -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//p'`
            value=`echo ${line} | awk -F= '{print $2}' | sed -n -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//p'`
            case "${key}" in
                output_path )
                    output_a[$idx]="${value}" ;;
                project_file )
                    project_a[$idx]="${value}" ;;
                options )
                    options_a[$idx]="${value}" ;;
                * )
                    echo "[${prod_nm_a[${idx}]}] unknown key: ${key}"
                    return 1
                    ;;
            esac
        fi
    done < ${1}

    return 0
}

cur_dir=`pwd`

# 构建目录
if [ -n "$(expr match "${0}" '\(.*/.*\)')" ]; then
    build_dir=${0%"/build.sh"}
    cd ${build_dir}
fi
build_dir=`pwd`

# 加载编译配置文件
load_config ${build_dir}/${COMPILE_CFG_FILE}
if [ $? -ne 0 ]; then
    exit 1
fi

# 选择编译产品
if [ -n "${1}" ]; then
    param=${1}
else
    OLDPS3=$PS3
    PS3='choose param: '
    select param in ${prod_nm_a[@]} "all" "clean" "quit"
    do
        echo
        if [ -z "$param" ];then
            echo "invalid choice, please choose again."
        else
            break
            echo
        fi
    done
    PS3=$OLDPS3
fi

# 编译选择的产品
if [ "${param}" = "clean" ]; then
    # 清除所有目标文件
    ls ${build_dir} | sed "s:^:`pwd`/:" | grep -v "build.sh$" | grep -v "${COMPILE_CFG_FILE}$" | grep -v "cmake$" | xargs rm -rf
    exit 0
elif [ "${param}" = "quit" ]; then
    exit 0
else
    idx=0
    while [ ${idx} -lt ${#prod_nm_a[@]} ]; do
        if [ "${param}" = "${prod_nm_a[${idx}]}" ] || [ "${param}" = "all" ]; then
            echo -e "\e[1;32m"
            echo "---------------------------------------------------"
            echo " Product     : ${prod_nm_a[${idx}]}"
            echo " Output path : ${output_a[${idx}]}"
            echo " Tool chain  : ${toolchain_a[${idx}]}"
            echo "---------------------------------------------------"
            echo -e "\e[0m"

            # 创建编译目标目录
            target_dir="${build_dir}/${output_a[${idx}]##*/}"
            mkdir ${target_dir} > /dev/null 2>&1

            # 编译
            cd ${target_dir}
            cmake_file_dir="${build_dir}/.."

            if [ -z "${options_a[${idx}]}" ]; then
                cmake ${cmake_file_dir} -DTARGET_PATH="${output_a[${idx}]}" -DPROJECT_FILE="${project_a[${idx}]}" -DFLINT=${NEED_FLINT}
            else
                cmake ${cmake_file_dir} -DTARGET_PATH="${output_a[${idx}]}" -DPROJECT_FILE="${project_a[${idx}]}" -DFLINT=${NEED_FLINT} -DOPTIONS="${options_a[${idx}]}"
            fi

            make -j
            if [ "$?" -ne "0" ]; then
                echo -e "\e[1;31mMake ${prod_nm_a[${idx}]} FAILED...\e[0m"
                cd ${cur_dir}
                exit 1
            else
                echo -e "\e[1;32mMake ${prod_nm_a[${idx}]} SUCCESS.\e[0m"
            fi

            if [ "${param}" != "all" ]; then
                break
            fi
        fi
        idx=$((idx+1))
    done

    if [ "${param}" != "all" ] && [ ${idx} -ge ${#prod_nm_a[@]} ]; then
        echo "no compile config for product \"${param}\" in ${COMPILE_CFG_FILE}"
    fi
fi

cd ${cur_dir}
exit 0
