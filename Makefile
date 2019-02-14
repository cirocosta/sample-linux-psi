build: src/cpu.out src/disk.out
setup: binaries services
services: /etc/systemd/system/psi_exporter.service /etc/systemd/system/node_exporter.service
binaries: /usr/local/bin/psi_exporter /usr/local/bin/node_exporter

image:
	docker build -t cirocosta/sample-psi .

upgrade-kernel:
	curl -SOL http://kernel.ubuntu.com/~kernel-ppa/mainline/v4.20/linux-headers-4.20.0-042000_4.20.0-042000.201812232030_all.deb
	curl -SOL http://kernel.ubuntu.com/~kernel-ppa/mainline/v4.20/linux-headers-4.20.0-042000-generic_4.20.0-042000.201812232030_amd64.deb
	curl -SOL http://kernel.ubuntu.com/~kernel-ppa/mainline/v4.20/linux-image-unsigned-4.20.0-042000-generic_4.20.0-042000.201812232030_amd64.deb
	curl -SOL http://kernel.ubuntu.com/~kernel-ppa/mainline/v4.20/linux-modules-4.20.0-042000-generic_4.20.0-042000.201812232030_amd64.deb
	dpkg -i *.deb
	update-grub

fmt:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=file -i

%.out: %.c
	gcc -Ofast $< -o $@

/etc/systemd/system/%.service:
	cp ./services/$*.service $@

/usr/local/bin/psi_exporter:
	curl -SL \
		https://github.com/cirocosta/psi_exporter/releases/download/v0.0.1/psi_exporter-linux-amd64.tgz | \
		tar xvz -C /tmp
	mv /tmp/psi_exporter.linux.out $@

/usr/local/bin/node_exporter:
	curl -SL \
		https://github.com/prometheus/node_exporter/releases/download/v0.17.0/node_exporter-0.17.0.linux-amd64.tar.gz | \
		tar xvz --strip-components=1 -C /tmp
	mv /tmp/node_exporter $@
