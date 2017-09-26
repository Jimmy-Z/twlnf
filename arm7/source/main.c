#include <nds.h>

//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

volatile bool exitflag = false;

//---------------------------------------------------------------------------------
void powerButtonCB() {
//---------------------------------------------------------------------------------
	exitflag = true;
}

//---------------------------------------------------------------------------------
int main() {
//---------------------------------------------------------------------------------
	readUserSettings();

	irqInit();
	// Start the RTC tracking IRQ
	initClockIRQ();
	fifoInit();
	touchInit();

	SetYtrigger(80);

	installSoundFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VcountHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT);
	
	setPowerButtonCB(powerButtonCB);   

	// Keep the ARM7 mostly idle
	while (!exitflag) {

		swiIntrWait(1,IRQ_FIFO_NOT_EMPTY);

		if (fifoCheckValue32(FIFO_USER_01)) {

			int command = fifoGetValue32(FIFO_USER_01);

			u64 consoleid;
			u32 nand_cid[4];
			u32 consoleid32[2];
			u32 aes_reg_cnt;

			switch(command) {
				case 4:
					sdmmc_nand_cid(nand_cid);
					fifoSendDatamsg(FIFO_USER_01, 16, (u8*)nand_cid);
					break;
				case 5:
					consoleid = REG_CONSOLEID;
					fifoSendDatamsg(FIFO_USER_01, 8, (u8*)&consoleid);
					break;
				case 6:
					consoleid32[0] = ((vu32*)(&REG_CONSOLEID))[0];
					consoleid32[1] = ((vu32*)(&REG_CONSOLEID))[1];
					fifoSendDatamsg(FIFO_USER_01, 8, (u8*)consoleid32);
					break;
				case 7:
					aes_reg_cnt = REG_AES_CNT;
					fifoSendDatamsg(FIFO_USER_01, 4, (u8*)&aes_reg_cnt);
					break;
			}

		}

		if ( 0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
			exitflag = true;
		}
	}
	return 0;
}
