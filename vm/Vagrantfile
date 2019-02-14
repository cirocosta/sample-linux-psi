# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"

  config.vm.provider "virtualbox" do |v|
    v.memory = 2048
    v.cpus = 2
  end

  config.vm.network "forwarded_port", guest: 9000, host: 9000
  config.vm.network "forwarded_port", guest: 9001, host: 9001
end
