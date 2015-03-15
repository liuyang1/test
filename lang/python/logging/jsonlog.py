import json
import logging
import logging.config

with open('logging.json') as fp:
    conf = json.load(fp)
    logging.config.dictConfig(conf)

logging.error("root log")
log = logging.getLogger('my_module')
log.debug("debug test again")
log.info("info test again")
log.warn("warn test again")
log.error("error test again")
