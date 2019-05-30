## Testing
### Local
start:
```
libnss-iam$ make bastion-run

or

libnss-iam$ make bastion-shell
root@bastion-service$ (cd /libnss-iam; make install)
root@bastion-service$ getent passwd <user>
<user>:*:65295:10:arn aws iam  <account> user/users/<user>:/home/<user>:/bin/bash
root@bastion-service$ /opt/sshd-entrypoint.sh
```

connect:
```
$ ssh [-i ~/.ssh/<key>] -p 2222 localhost
```

## Fully Damic Users
NOTE: 
```
not possible, this requires 2 successive logins 
- won't work in our 'new' container per connection environment

pam_exec.so env:
PAM_RHOST=172.17.0.1
PAM_SERVICE=sshd
PAM_TTY=ssh
PAM_TYPE=auth
PAM_USER=<username>
PWD=/
```

/etc/pam.d/sshd:
```
...
auth optional pam_exec.so stdout /opt/create-user-command.sh
...
```
/etc/ssh/sshd_config:
```
...
AuthenticationMethods publickey keyboard-interactive:pam,publickey
...
```

## Links
Reviewed, inspired by:
* https://docs.aws.amazon.com/quickstart/latest/linux-bastion/architecture.html
* https://medium.com/@joshuamkite/the-bastion-server-that-isnt-there-87a5c18c3c75
  * https://registry.terraform.io/modules/joshuamkite/ssh-bastion-service/aws/4.9.0
  * https://github.com/joshuamkite/terraform-aws-ssh-bastion-service
* https://github.com/widdix/aws-ec2-ssh/
* https://medium.com/@jryancanty/just-in-time-ssh-provisioning-7b20d9736a07
* https://github.com/kislyuk/keymaker
* https://serverfault.com/questions/677926/authorizedkeyscommand-in-openssh-without-user
* https://www.freebsd.org/cgi/man.cgi?sshd_config(5)
* http://www.linux-pam.org/Linux-PAM-html/sag-pam_exec.html

## MOTD Sample
```
Warning: Permanently added '[localhost]:2222' (ECDSA) to the list of known hosts.
Welcome to Ubuntu 18.04.2 LTS (GNU/Linux 4.4.0-146-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage
This system has been minimized by removing packages and content that are
not required on a system that users do not log into.

To restore this content, you can run the 'unminimize' command.

The programs included with the Ubuntu system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Ubuntu comes with ABSOLUTELY NO WARRANTY, to the extent permitted by
applicable law.

Environment:
  LANG=en_US.UTF-8
  USER=fvogt
  LOGNAME=fvogt
  HOME=/home/fvogt
  PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
  MAIL=/var/mail/fvogt
  SHELL=/bin/bash
  TZ=UTC
  SSH_CLIENT=172.17.0.1 56770 22
  SSH_CONNECTION=172.17.0.1 56770 172.17.0.2 22
  SSH_TTY=/dev/pts/1
  TERM=xterm-256color
To run a command as administrator (user "root"), use "sudo <command>".
See "man sudo_root" for details.
```
