require 'rake'
require 'rake/clean'

task :default => :build

task :cruise => [:clean, :pkgs, :install]

file 'configure' => ['configure.in', 'autogen.sh']  do |t|
  system( "cd #{File.dirname __FILE__} && ./autogen.sh" ) and $?.exitstatus.zero?  or raise "Autogen failure"
end
CLEAN.include 'configure'

file 'libFinagle.spec' => ['libFinagle.spec.in', 'configure']  do
  system( "cd #{File.dirname __FILE__} && ./configure" ) and $?.exitstatus.zero?  or raise "Configure failure"
end


desc 'Build libFinagle'
task :build => 'libFinagle.spec'  do |t|
  system( "make -C #{File.dirname __FILE__}" ) and $?.exitstatus.zero?  or raise "Make failed"
end

desc 'Build all libFinagle packages'
task :pkgs => 'libFinagle.spec'  do |t|
  system( "make -C #{File.dirname __FILE__} pkgs" ) and $?.exitstatus.zero?  or raise "Make failed"
end

#desc 'Install libFinagle packages'
#task :install => :pkgs  do |t|
#  system( "rpm -Uvh ..." ) and $?.exitstatus.zero?  or raise "Make failed"
#end



#RPMDIR='/var/www/html/rpms'
#make pkgs-install RPMDIR="$RPMDIR" &&
#repomanage --keep 1 --old --space $RPMDIR | xargs rm -f  &&
#rpm --resign $RPMDIR/libFinagle*  && 
#createrepo $RPMDIR  &&
#sudo yum makecache  && 
#sudo yum update -y libFinagle libFinagle-devel  &&

