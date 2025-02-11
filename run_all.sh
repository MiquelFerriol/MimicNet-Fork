#!/bin/bash

set -e
set -x

if [[ -z ${2} ]]; then
    echo "Usage: run_all.sh variant num_clusters [simulate_options]"
    exit 1
fi

rm -f -r simulate/simulate_tcp/results/*
rm -f -r simulate/simulate_mimic_tcp/results/*
rm -f -r data/

VARIANT=${1}
NUM_CLUSTERS=${2}

./run_1_compile.sh CPU

exec 6>&1
RESULTS_DIR=$(./run_2_generate.sh ${VARIANT} ${@:3} | tee /dev/fd/6 | tail -n 1)

exec 7>&1
MODEL_STR=$(./run_3_hypertrain.sh ${VARIANT} train/lstm/train_lstm_${VARIANT}.py \
                    ${RESULTS_DIR}  tune/hp_configs/lstm_${VARIANT}.json ${@:3} | tee /dev/fd/7 | tail -n 1)

#./run_4_mimicnet.sh ${VARIANT} ${MODEL_STR} ${NUM_CLUSTERS} ${@:3}


#./run_3_hypertrain.sh tcp train/lstm/train_lstm_tcp.py data/sw2_sv4_l0.70_L100e6_s0_qDropTailQueue_vTCPNewReno_S20_tcp tune/hp_configs/lstm_tcp.json