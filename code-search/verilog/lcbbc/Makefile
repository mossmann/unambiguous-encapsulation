
# Target project
TARGET = lcbbc

# Assume that quartus_cmd and quartus_pgm are in $PATH unless
QUARTUS_CMD = quartus_cmd
QUARTUS_PGM = quartus_pgm
QUARTUS_CPF = quartus_cpf

OUTPUT_DIR = output
OUTPUT_FILE = $(OUTPUT_DIR)/$(TARGET).sof
SETTINGS_FILE = $(TARGET).qsf

all: $(OUTPUT_FILE)

$(OUTPUT_FILE):
	$(QUARTUS_CMD) $(TARGET) -c $(SETTINGS_FILE)
	
program: $(OUTPUT_DIR)/$(TARGET).sof
	$(QUARTUS_PGM) --mode=jtag -o "P;$(OUTPUT_FILE)@1"

flash: $(TARGET).opt $(OUTPUT_DIR)/$(TARGET).sof
	$(QUARTUS_CPF) --convert --option=$(TARGET).opt --device=EPCS64 $(OUTPUT_DIR)/$(TARGET).sof $(TARGET).pof
	#$(QUARTUS_PGM) --mode=jtag -o "P;$(OUTPUT_FILE)@1"
	
optionfile:
	$(QUARTUS_CPF) -w $(TARGET).opt

clean:
	rm -rf output db incremental_db

.PHONY: all clean program $(OUTPUT_FILE)
