Project.configure do |project|
  project.email_notifier.emails = %w/steve@finagle.org/
  project.build_command = %q{
    RPMDIR='/var/www/html/rpms' ;
    make pkgs-install RPMDIR="$RPMDIR" &&
    repomanage --keep 1 --old --space $RPMDIR | xargs rm -f  &&
    rpm --resign $RPMDIR/libFinagle*  &&
    createrepo $RPMDIR  &&
    sudo rpm -Uvh $RPMDIR/libFinagle*
  }

#  project.rake_task = ''
#  project.email_notifier.from = ''
#  project.scheduler.polling_interval = 5.minutes
end

