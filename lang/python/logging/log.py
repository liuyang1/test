import logging
# logging.basicConfig(level=logging.INFO)
log = logging.getLogger("logtest")
log.setLevel(logging.WARNING)
log.debug("debug test")
log.info("info test")
log.warn("warn test")
log.error("error test")

hdl = logging.FileHandler('logtest.log')
hdl.setLevel(logging.INFO)
fmt = logging.Formatter('%(asctime)s %(levelname)-7s - %(message)s')
hdl.setFormatter(fmt)
log.addHandler(hdl)

log.debug("debug test again")
log.info("info test again")
log.warn("warn test again")
log.error("error test again")
