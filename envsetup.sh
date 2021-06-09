_LIBAROMA_ENVSETUP_ROOT="$(readlink -f $(dirname $0))"

if [ ! -e "${_LIBAROMA_ENVSETUP_ROOT}/tools/linux/_aroma_envsetup.sh" ]; then
	echo "fatal error: linux build tools not accessible!"
	exit
fi

. "${_LIBAROMA_ENVSETUP_ROOT}/tools/linux/_aroma_envsetup.sh"
