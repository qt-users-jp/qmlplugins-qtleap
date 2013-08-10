%modules = ( # path to module name map
    "QtLeap" => "$basedir/src/lib",
);

%moduleheaders = ( # restrict the module headers to those found in relative path
);

%classnames = (
    "qleaphandmodel.h" => "QLeapHandModel"
);
%dependencies = (
    "qtbase" => "refs/heads/dev",
);
