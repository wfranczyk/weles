#include <io/Ata.h>
#include <io/Vga.h>

bool init()
{
  Vga vga;
  vga.print("Kernel bootstrap\n");
  vga.print("VGA Initialized\n");

  vga.print("Initializing ATA controller... ");
  Ata ata;
  switch (ata.init())
  {
  case Ata::InitStatus::NoDrivesDetected:
    vga.print("Error: no drives has been detected");
    return false;
  case Ata::InitStatus::NoPrimaryMaster:
    vga.print("Error: Primary Master has not been detected");
    return false;
  case Ata::InitStatus::NotAtaDevice:
    vga.print("Error: Primary Master is not an ATA device");
    return false;
  case Ata::InitStatus::Error:
    vga.print("Error: Failure during Primary Master initialization");
    return false;
  default:
    vga.print("Done\n");
  }

  auto& di = ata.test();
  di.serialNumber[19] = 0;
  di.firmwareRevision[7] = 0;
  di.modelNumber[39] = 0;
  vga.print("ATA Serial Number: ");
  vga.print(di.serialNumber);
  vga.print("\n");
  vga.print("ATA Firmware: ");
  vga.print(di.firmwareRevision);
  vga.print("\n");
  vga.print("ATA Model: ");
  vga.print(di.modelNumber);
  vga.print("\n");


}

int bmain()
{
  init();
  for (;;)
  {
  }

  return 0;
}
