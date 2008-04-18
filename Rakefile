gem 'rake-tasks'

require 'rake'
require 'rake/autotoolstask'

task :default => :pkgs

task :cruise => [:clean, :pkgs]

desc 'Build libFinagle packages'
Rake::AutoToolsRPMTask.new 'libFinagle:pkg' => File.join( File.dirname(__FILE__), 'libFinagle.spec' )
task :pkgs => 'libFinagle:pkg'


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

