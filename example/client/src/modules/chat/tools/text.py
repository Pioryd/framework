import fw_module_manager
import fw_core_logger
import signal
import sys

def int():
  fw_module_manager.connect("onKeyDown", keyDown)

def keyDown(keyCode):
  fw_core_logger.info("keyDown - KeyCode: [%s]." % keyCode)
  fw_core_logger.info("keyDown v10 - chat.text.keyDown")

def unBindKeyDown(keyCode):
  fw_core_logger.info("unBindKeyDown - KeyCode: [%s]." % keyCode)
  fw_module_manager.disconnect("onKeyDown", keyDown)