export SEED=$RANDOM
export LD_PRELOAD=$PWD/wrappers/$1_wrapper.so 
${@:2}
export LD_PRELOAD=
