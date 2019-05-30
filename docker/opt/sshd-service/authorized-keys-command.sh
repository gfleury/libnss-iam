#!/bin/bash

set -eu -o pipefail

export AWS_PROFILE=admin-main

username=${1?Usage: $0 <username>}
username=$(echo "$username" | sed -e 's/^# //' -e 's/+/plus/' -e 's/=/equal/' -e 's/,/comma/' -e 's/@/at/' )

(
sshkey_id=$(
    aws iam list-ssh-public-keys \
        --user-name "$username" \
        --query "SSHPublicKeys[?Status == 'Active'].[SSHPublicKeyId]" \
        --output text
)
ssh_key=$(
    aws iam get-ssh-public-key \
    --user-name "$username" \
    --ssh-public-key-id "$sshkey_id" \
    --encoding SSH \
    --query "SSHPublicKey.SSHPublicKeyBody" \
    --output text
)

if ! grep "^${username}:" /etc/passwd; then
    getent passwd fvogt | tail -n 1 >> /etc/passwd
fi
mkdir -p "/home/${username}"
for file in .bashrc .profile .bash_logout; do
    if [[ ! -f "/etc/skel/${file}" ]]; then
        cp -r "/etc/skel/${file}" "/home/${username}"
    fi
done

echo "$username ALL=(ALL) NOPASSWD:ALL" > "/etc/sudoers.d/${username}"
chmod 0440 "/etc/sudoers.d/${username}"

mkdir -p "/home/${username}/.ssh"
if ! grep "$ssh_key" "/home/${username}/.ssh/authorized_keys"; then
    echo "$ssh_key" >> "/home/${username}/.ssh/authorized_keys"
fi
chown -R "${username}" "/home/${username}/.ssh"
chmod 700 "/home/${username}/.ssh"
chmod 0600 "/home/${username}/.ssh/authorized_keys"
) > /dev/null 2>&1

cat "/home/${username}/.ssh/authorized_keys"
