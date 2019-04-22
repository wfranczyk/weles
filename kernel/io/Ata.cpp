#include "Ata.h"

#include <io/PortIo.h>
#include <libk/cstdlib>

/* bool Ata::m_initialized = false; */

Ata::InitStatus Ata::init()
{
  if (m_initialized)
    return InitStatus::Ok;

  constexpr unsigned noDrivesByte = 0xff;
  if (inb(RegStatus) == noDrivesByte)
    return InitStatus::NoDrivesDetected;

  constexpr unsigned driveSelectorMaster = 0xa0;
  outb(RegDriveHead, driveSelectorMaster); // select primary master, the only supported for now

  // ATA specs says that it's good to give some time to the drive before
  // executing a next command once the drive selection has been performed.
  // They suggest to read the status 5 times, the last one is real status
  // value. Despite that we don't need the status now, let's wait for the
  // selection to finish. Just to be sure, we're not in hurry now.
  inb(RegStatus);
  inb(RegStatus);
  inb(RegStatus);
  inb(RegStatus);
  inb(RegStatus);
  
  // reset sector selectors 
  outb(RegSectorCount, 0);
  outb(RegSectorNumber, 0);
  outb(RegLbaLow, 0);
  outb(RegLbaHigh, 0);
  
  // identify
  outb(RegCommand, CmdIdentify);
  if (inb(RegStatus) == 0)
    return InitStatus::NoPrimaryMaster;

  // wait for not busy
  while ((inb(RegStatus) & StBsyBit) != 0);

  if (inb(RegLbaLow) != 0 || inb(RegLbaHigh) != 0)
    return InitStatus::NotAtaDevice;

  // wait for the drive finish initialization until done or err
  while ((inb(RegStatus) & StDrqBit) == 0 &&
         (inb(RegStatus) & StErrBit) == 0);

  if (inb(RegStatus) & StErrBit)
    return InitStatus::Error;

  inwn(RegData,
       reinterpret_cast<std::uint16_t*>(&m_driverIdentifier),
       sizeof(DriverIdentifier) / sizeof(std::uint16_t));

  m_initialized = true;
  return InitStatus::Ok;
}
