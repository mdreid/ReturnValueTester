./set_seed &
SEED_ID=$!
LD_PRELOAD=$PWD/wrappers/$1_wrapper.so ${@:2}
kill $SEED_ID
