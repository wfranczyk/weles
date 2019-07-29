#include "Shell.h"

#include "Key.h"

#include <stdio.h>
#include <string.h>

void Shell::init()
{
  printf("Skorupa Welesa v0.2\n");
  showPrompt();
}

auto Shell::keyboardEvent(unsigned char event) -> EventRes
{
  handleVga(event);
  if (m_mode == Mode::PrintOnly)
    return EventRes::Meh;

  return handleActions(event);
}

void Shell::cleanup()
{
  memset(m_cmdBuffer, 0, sizeof(m_cmdBuffer));
  m_cmdBufferIdx = 0;
}

void Shell::addToBuffer(char c)
{
  if (m_cmdBufferIdx < sizeof(m_cmdBuffer))
  {
    m_cmdBuffer[m_cmdBufferIdx++] = c;
  }
}

void Shell::bufferRetreat()
{
  if (m_cmdBufferIdx > 0)
    --m_cmdBufferIdx;
  
  m_cmdBuffer[m_cmdBufferIdx] = 0;
}

auto Shell::handleActions(unsigned char event) -> EventRes
{
  if (event > Key::SpecialKeysStart)
  {
    if (event == Key::Backspace)
    {
      bufferRetreat();
    }
  }
  else
  {
    if (event == '\n')
    {
      if (m_cmdBufferIdx > 0)
        return EventRes::ImSoProud_LaunchTheMissile;
      else
        showPrompt();
    }
    else
    {
      addToBuffer(event);
    }
  }

  return EventRes::Meh;
}

void Shell::handleVga(unsigned char event)
{
  if (event > Key::SpecialKeysStart)
  {
    if (event == Key::Backspace)
    {
      m_vga.retreat();
    }
  }
  else
  {
    printf("%c", event);
  }
}

void Shell::showPrompt()
{
  printf("$ ");
}
