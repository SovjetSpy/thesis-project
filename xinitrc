# xinitrc file for testing our window manager.


# 1. Start some programs to play with.
# sleep 1 && xclock &
# xeyes &
sleep 1 && xterm &
sleep 1 && xclock &

# 2. Start our window manager.
export GLOG_logtostderr=1
exec ./a.out