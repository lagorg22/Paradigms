include $(GL_LIB_PATH)/makefile_base

tests: tests.c
	$(CC) $(CFLAGS) -c $< -o $(OUTDIR)/$@.o 

run_tests: $(OUTDIR)/vector.o $(OUTDIR)/map_list.o tests
	OUT_DIR=$(OUTDIR) make -C $(GL_LIB_PATH)/lunit lunit
	$(LD) $(LDFLAGS) $(OUTDIR)/tests.o $(OUTDIR)/vector.o $(OUTDIR)/map_list.o $(OUTDIR)/lunit.o $(OUTDIR)/cmp.o $(OUTDIR)/list.o -o $(OUTDIR)/$@
