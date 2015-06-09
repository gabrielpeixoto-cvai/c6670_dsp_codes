/*
 * fftc_linker.cmd
 *
 *  Created on: 08/05/2014
 *      Author: igorfreire
 */


SECTIONS
{
    .init_array  >       L2SRAM
    .text        >       L2SRAM
    .stack       >       L2SRAM
    .bss         >       L2SRAM
    .cinit       >       L2SRAM
    .cio         >       L2SRAM
    .const       >       L2SRAM
    .data        >       L2SRAM
    .switch      >       L2SRAM
    .sysmem      >       L2SRAM
    .far         >       L2SRAM
    .ppdata      >       L2SRAM
    .qmss: load >> MSMCSRAM
    .cppi: load >> MSMCSRAM
    //.fftc: load >> DDR3
    //cppiSharedHeap: load >> MSMCSRAM

    .init_array: load >> L2SRAM
}
