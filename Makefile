CC	= ./gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG

BINS	= gbghost.gb

all: $(BINS)

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

%.gb:	%.o
	$(CC) -o $@ $<

clean:
	rm -f *.o *.lst *.map *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm
