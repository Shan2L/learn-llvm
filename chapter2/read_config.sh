testcase=$(cat config.yml | shyaml get-value testcase)
dtype_list=$(echo "${testcase}" | shyaml keys)
echo ${dtype_list}

for dtype in ${dtype_list}
do 
    dtype_struc=$(cat config.yml | shyaml get-value testcase | shyaml get-value ${dtype})
    case_list=$(echo "${dtype_struc}" | shyaml keys)

    for case in ${case_list}
    do
        param_type=$(echo "${dtype_struc}"| shyaml get-type ${case})
        if [[ ${param_type} == "sequence" ]]; then
            param_length=$(echo "${dtype_struc}" | shyaml get-length ${case})
            for ((i=0; i<${param_length}; i++))
            do
                param=$(echo "${dtype_struc}" | shyaml get-value ${case}.${i})
                echo ${param}
            done
        elif [[ ${param_type} == "NoneType" ]]; then
            param=''
            echo ${param}
        fi
        # param_type=$(cat config.yml | shyaml get-value testcase | shyaml get-value ${dtype} | shyaml get-type)
    done
    echo '-----------'
done
