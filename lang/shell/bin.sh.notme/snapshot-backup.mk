#!/usr/bin/make -f
#
# Backup snapshots, based on rsync.
#
# usage:
# 	mkdir backup && cd backup
# 	echo "server:dir" > source
# 	snapshot-backup.mk
#
# 2007.2  Fengguang Wu <wfg@ustc.edu>
#

SRC	:= $(shell cat source)
DST	:= $(shell date +'%Y-%m-%d')
CURRENT	:= $(shell readlink current)
EXCLUDE	:= $(wildcard exclude)
FILES	:= $(wildcard files)

LINK_DEST	:= $(if $(CURRENT),--link-dest=../$(CURRENT))
EXCLUDE_FROM	:= $(if $(EXCLUDE),--exclude-from=$(EXCLUDE))
FILES_FROM	:= $(if $(FILES),--files-from=$(FILES))

BASIC_OPTIONS	:= -av --delete --numeric-ids
RSYNC_OPTIONS	:= $(BASIC_OPTIONS) $(FILES_FROM) $(EXCLUDE_FROM)

daily:
	@test "$(SRC)"
	rsync $(RSYNC_OPTIONS) $(LINK_DEST) $(SRC) $(DST)
	rm -f current
	ln -s $(DST) current


N=9
rotate:
	rm -fr $N
	for i in `seq $N -1 1`; do let j=i-1; test -d $$j && mv -f $$j $$i || true; done
	rsync $(RSYNC_OPTIONS) --link-dest=../1 $(SRC) 0

