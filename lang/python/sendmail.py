import sys
import smtplib

def buildMsg(fr, to, subject, text):
    # need this format as email protocol
    return """From: %s\nTo: %s\nSubject: %s\n\n%s\n\n""" \
        % (fr, ", ".join(to), subject, text)


def sendmail(fr, to, subject, text, ServerAddr, password):
    message = buildMsg(fr, to, subject, text)
    print(message)
    server = smtplib.SMTP(ServerAddr)
    server.login(fr, password)
    server.sendmail(fr, to, message)
    server.quit()
