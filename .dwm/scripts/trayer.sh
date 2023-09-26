if [ "$(ps ax|grep -v grep|grep -v scripts|grep trayer)" == "" ] ; then
	eval "trayer --align center --width 30 --SetDockType false --transparent true --tint 0x88888888 --expand false &"
else
	eval "killall trayer"
fi
