/*
DBHOST "tcp://127.0.0.1:3306"
USER "root"
PASSWORD "bird"
DATABASE "php-srs"

NOTE: These can be changed in command line arguments
*/

CREATE TABLE `products` (
  `ProductID` int(11) NOT NULL AUTO_INCREMENT,
  `Name` varchar(45) DEFAULT NULL,
  `Price` decimal(8,2) DEFAULT NULL,
  `Stock` int(11) DEFAULT NULL,
  PRIMARY KEY (`ProductID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='List of all products that can be sold'

CREATE TABLE `individual_sale` (
  `SaleID` int(11) NOT NULL,
  `ProductID` int(11) NOT NULL,
  `Quanity` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`SaleID`,`ProductID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8

CREATE TABLE `sales_record` (
  `SalesRecordID` int(11) NOT NULL AUTO_INCREMENT,
  `DateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `SalePrice` decimal(8,2) DEFAULT NULL,
  PRIMARY KEY (`SalesRecordID`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8