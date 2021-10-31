<?php

#
# QAS Pro Web - PHP Integration code
# (c) QAS Ltd 2004
#
# qaddress.inc - QuickAddress common class for PHP
#


# Constants

define( "CONTROL_WSDL_URN",         "http://127.0.0.1:2021/proweb.wsdl" );
// define( "CONTROL_WSDL_URN", "http://10.90.14.3:2021/proweb.wsdl");
define( "DATA_ID",                  "GBR" );

# Layout with which to format addresses
define( "CONTROL_LAYOUT",           "XI" );
define( "QAS_SINGLELINE_ENGINE",    "Singleline" );
define( "QAS_TYPEDOWN_ENGINE",      "Typedown" );
define( "QAS_VERIFICATION_ENGINE",  "Verification" );

define( "QAS_EXACT_SEARCHING",      "Exact" );
define( "QAS_CLOSE_SEARCHING",      "Close" );
define( "QAS_EXTENSIVE_SEARCHING",  "Extensive" );

define( "QAS_ONELINE_PROMPT",       "OneLine" );
define( "QAS_DEFAULT_PROMPT",       "Default" );
define( "QAS_GENERIC_PROMPT",       "Generic" );
define( "QAS_OPTIMAL_PROMPT",       "Optimal" );
define( "QAS_ALTERNATE_PROMPT",     "Alternate" );
define( "QAS_ALTERNATE2_PROMPT",    "Alternate2" );
define( "QAS_ALTERNATE3_PROMPT",    "Alternate3" );

define( "QAS_LINE_SEPARATOR",       "|" );

# Class definitions

class Picklist {
	var $iTotal = 0;
	var $sPicklistMoniker = "";
	var $sPrompt = "No Items";
	var $atItems = array();
	var $isTimeout;
	var $isMaxMatches;
	var $bOverThreshold;
	var $bLargePotential;
	var $bMoreOtherMatches;
	var $bAutoStepinSafe;
	var $bAutoStepinPastClose;
	var $bAutoFormatSafe;
	var $bAutoFormatPastClose;

	# Picklist constructor - make sure that the picklist items are consistently an array
	#
	function Picklist( $result ) {
		if ( QuickAddress::check_soap( $result ) != NULL && ( $tPicklist = $result->QAPicklist ) != NULL ) {
			$this->iTotal               = $tPicklist->Total;
			$this->sPrompt              = $tPicklist->Prompt;
			$this->sPicklistMoniker     = $tPicklist->FullPicklistMoniker;
			$this->isTimeout            = $tPicklist->Timeout;
			$this->isMaxMatches         = $tPicklist->MaxMatches;
			$this->bOverThreshold       = $tPicklist->OverThreshold;
			$this->bLargePotential      = $tPicklist->LargePotential;
			$this->bMoreOtherMatches    = $tPicklist->MoreOtherMatches;
			$this->bAutoStepinSafe      = $tPicklist->AutoStepinSafe;
			$this->bAutoStepinPastClose = $tPicklist->AutoStepinPastClose;
			$this->bAutoFormatSafe      = $tPicklist->AutoFormatSafe;
			$this->bAutoFormatPastClose = $tPicklist->AutoFormatPastClose;

			if ( !isset( $tPicklist->PicklistEntry ) )
				$this->atItems = array();
			elseif ( is_array( $tPicklist->PicklistEntry ) )
				$this->atItems = $tPicklist->PicklistEntry;
			else
				$this->atItems = array( $tPicklist->PicklistEntry );
		}
	}

	# Is a picklist object suitable for auto-stepin?
	#
	function isAutoStepinSingle() {
		return( $this->iTotal == 1 &&
				$this->atItems[ 0 ]->CanStep &&
				!$this->atItems[ 0 ]->Information );
	}

	# Is a picklist object suitable for auto-format?
	#
	function isAutoFormatSingle() {
		return( $this->iTotal == 1 &&
				$this->atItems[ 0 ]->FullAddress &&
				!$this->atItems[ 0 ]->Information );
	}
}

class QASAddress {
	var $atAddressLines;
	var $sVerifyLevel;

	# Address constructor - make sure that address lines are consistently an array
	#
	function Address( $tQAAddress ) {
		$this->atAddressLines = $tQAAddress->AddressLine;

		if ( !is_array( $this->atAddressLines ) )
			$this->atAddressLines = array( $this->atAddressLines );
	}
}

class FormattedAddress extends QASAddress {
	# FormattedAddress constructor - check for SOAP errors then call Address constructor
	#
	function FormattedAddress( $result ) {
		if ( QuickAddress::check_soap( $result ) != NULL ) {
			parent::Address( $result->QAAddress );
		}
	}
}

class Examples {
	var $atAddress = NULL;
	var $asComment = NULL;

	# Examples constructor - check for SOAP errors, then build an address/comment array
	#
	function Examples( $result ) {
		if ( QuickAddress::check_soap( $result ) != NULL ) {
			if ( is_array( $result->ExampleAddress ) ) {
				foreach( $result->ExampleAddress AS $tAddress ) {
					$this->atAddress[] = new Address( $tAddress->Address );
					$this->asComment[] = $tAddress->Comment;
				}
			}
			else {
				$this->atAddress[] = new Address( $result->ExampleAddress->Address );
				$this->asComment[] = $result->ExampleAddress->Comment;
			}
		}
	}
}

class SearchResult {
	var $picklist;
	var $address;
	var $sVerifyLevel;

	# SearchResult constructor - check SOAP then attach picklist & address objects
	#
	function SearchResult( $result ) {
		if ( QuickAddress::check_soap( $result ) != NULL ) {
			if ( isset( $result->QAPicklist ) )
				$this->picklist = new Picklist( $result );

			if ( isset( $result->QAAddress ) ) {
				$this->address = new FormattedAddress( $result );
			}

			$this->sVerifyLevel = $result->VerifyLevel;
		}
	}
}

class PromptSet {
	var $atLines = NULL;

	# PromptSet constructor - check SOAP then make sure that prompt lines are consistently an array
	#
	function PromptSet( $LineIn ) {
		if ( QuickAddress::check_soap( $LineIn ) != NULL ) {
			if ( is_array( $LineIn ) )
				$this->atLines = $LineIn;
			else
				$this->atLines = array( $LineIn );
		}
	}
}

class QuickAddress {
	var $sEngineType      = QAS_SINGLELINE_ENGINE;
	var $sConfigFile      = "";
	var $sConfigSection   = "";
	var $sEngineIntensity = "";
	var $iThreshold       = 0;
	var $iTimeout         = -1;
	var $bFlatten         = FALSE;
	var $soap = NULL;

	# QuickAddress constructor - suppress exception generation as we want to keep this integration code
	# PHP4 compatible
	#
	function QuickAddress( $sEndpointURL ) {
		$this->soap = new SoapClient( $sEndpointURL, array( "exceptions" => 0 ) );

		if ( is_soap_fault( $this->soap ) ) {
			$this->soap = NULL;
		}
	}

	# Check a result for a soap fault object, and log it to the PHP log channel
	#
	public static function check_soap( $soapResult )
	{
		if ( is_soap_fault( $soapResult ) ) {
			$err = "QAS SOAP Fault - ".
				   "Code: {".$soapResult->detail->QAFault->ErrorCode."}, ".
				   "Description: {".$soapResult->detail->QAFault->ErrorMessage."}";

			error_log( $err, 0 );

			$soapResult = NULL;
		}

		return( $soapResult );
	}

	# Get the last QAS Soap fault
	#
	function getSoapFault() {
		return( isset( $this->soap->__soap_fault )
			? $this->soap->__soap_fault->detail->QAFault
			: NULL );
	}

	# Setup a fault string for display
	#
	function getFaultString( $sFault ) {
		if ( ( !is_string( $sFault ) || $sFault == "" ) && ( $this->getSoapFault() != NULL ) )
			return( $this->getSoapFault()->ErrorMessage . " [" . $this->getSoapFault()->ErrorCode . "]" );
		else
			return( $sFault );
	}

	# Set the engine type to use
	#
	function setEngineType( $sType ) {
		$this->sEngineType = $sType;
	}

	# Set the engine intensity
	#
	function setEngineIntensity( $sIntensity ) {
		$this->sEngineIntensity = $sIntensity;
	}

	# Set the picklist threshold
	#
	function setThreshold( $iThreshold ) {
		$this->iThreshold = $iThreshold;
	}

	# Set the search/refinement timeout
	#
	function setTimeout( $iTimeout ) {
		$this->iTimeout = $iTimeout;
	}

	# Set flattened mode
	#
	function setFlatten( $bFlatten ) {
		$this->bFlatten = $bFlatten;
	}

	# Set configuration file to use
	#
	function setConfigFile( $sConfig ) {
		$this->sConfigFile = $sConfig;
	}

	# Set configuration section to use
	#
	function setConfigSection( $sSection ) {
		$this->sConfigSection = $sSection;
	}

	# Get a list of available data sets - and check that the result is consistently an array
	#
	function getAllDataSets() {
		$result = $this->check_soap( $this->soap->DoGetData() );

		if ( $result != NULL ) {
			$result = $result->DataSet;

			if ( is_array( $result ) )
				return( $result );
			else
				return( array( $result ) );
		}
		else
			return( NULL );
	}

	# Get a list of available layouts - and check that the result is consistently an array
	#
	function getLayouts( $sDataSetID ) {
		$result = $this->check_soap( $this->soap->DoGetLayouts( array( "Country" => $sDataSetID ) ) );

		if ( $result != NULL ) {
			$result = $result->Layout;

			if ( is_array( $result ) )
				return( $result );
			else
				return( array( $result ) );
		}
		else
			return( array() );
	}

	# Test whether a search can be performed for a layout/dataset combination by checking licensing, etc.
	#
	# Return the result object on success, else FALSE
	#
	function canSearch( $sDataSetID, $sLayoutName ) {
		$result = $this->check_soap( $this->soap->DoCanSearch( array( "Country" => $sDataSetID,
																	  "Engine"  => $this->sEngineType,
																	  "Layout"  => $sLayoutName ) ) );
		if ( $result != NULL && $result->IsOk )
			return( $result );
		else
			return( FALSE );
	}

	# Perform an initial search
	#
	# Parameters:
	#   sDataSetID      ID of the dataset to be searched
	#   asSearch        array of search terms
	#   sPromptSet      (optional) Name of the prompt set used for these search terms
	#   sVerifyLayout   (optional) Name of the output layout (verification mode only)
	#
	# Return a picklist containing the results of the search
	#
	function search( $sDataSetID, $asSearch, $sPromptSet = NULL, $sVerifyLayout = NULL ) {
		$this->sDataSetID = $sDataSetID;

		# Concatenate each line of input to a search string delimited by line separator characters
		$sSearchString = "";
		$bFirst = TRUE;
		foreach( $asSearch AS $sSearch ) {
			if ( !$bFirst ) {
				$sSearchString = $sSearchString . QAS_LINE_SEPARATOR;
			}
			$sSearchString = $sSearchString . $sSearch;
			$bFirst = FALSE;
		}

		# Set engine type and options - "_" is reserved by PHP SOAP to indicate the
		# tag value while the other elements of the array set attribute values
		$aEngineOptions = array( "_" => $this->sEngineType, "Flatten" => $this->bFlatten );

		# Set prompt set if not default
		if ( $sPromptSet != NULL )
			$aEngineOptions[ "PromptSet" ] = $sPromptSet;

		# Set threshold if not default
		if ( $this->iThreshold != 0 )
			$aEngineOptions[ "Threshold" ] = $this->iThreshold;

		# Set timeout if not default
		if ( $this->iTimeout != -1 )
			$aEngineOptions[ "Timeout" ] = $this->iTimeout;

		# Build main search arguments
		$args = array( "Country" => $this->sDataSetID,
					   "Search"  => $sSearchString,
					   "Engine"  => $aEngineOptions );

		# Are we using a non-default configuration file or section ?
		# then setup the appropriate tags
		if ( $this->sConfigFile != "" || $this->sConfigSection != "" ) {
			$asConfig = array();

			if ( $this->sConfigFile != "" )
				$asConfig[ "IniFile" ] = $this->sConfigFile;

			if ( $this->sConfigSection != "" )
				$asConfig[ "IniSection" ] = $this->sConfigSection;

			$args[ "QAConfig" ] = $asConfig;
		}

		# Set layout (for verification engine) if not default
		if ( $sVerifyLayout != NULL )
			$args[ "Layout" ] = $sVerifyLayout;

		# Perform the web service call and create a SearchResult instance with the result
		return( new SearchResult( $this->soap->DoSearch( $args ) ) );
	}

	# Perform an initial search using the Singleline engine, returning a picklist.
	#
	# Parameters:
	#   sDataSetID  ID of the dataset to be searched
	#   asSearch    array of search terms
	#
	# Return a Picklist item
	#
	function searchSingleline( $sDataSetID, $asSearch, $sPromptSet = NULL ) {
		$engineOld = $this->sEngineType;
		$this->sEngineType = QAS_SINGLELINE_ENGINE;

		$searchResult = $this->search( $sDataSetID, $asSearch, $sPromptSet );
		$this->sEngineType = $engineOld;

		return( $searchResult->picklist );
	}

	# Perform a refinement
	#
	# Parameters:
	#   sRefinementText     Text on which to refine
	#   sMoniker            Search point moniker of the picklist being refined
	#
	# Return A picklist instance containing the results of the refinement
	#
	function refine( $sMoniker, $sRefinementText ) {
		$args = array( "Moniker" => $sMoniker, "Refinement" => $sRefinementText );

		if ( $this->iThreshold != 0 ) {
			$args[ "Threshold" ] = $this->iThreshold;
		}
		if ( $this->iTimeout != -1 ) {
			$args[ "Timeout" ] = $this->iTimeout;
		}

		return( new Picklist( $this->soap->DoRefine( $args ) ) );
	}

	# Perform a step-in
	#
	# Parameters:
	#   sMoniker    The search point moniker of the picklist item to be entered
	#
	# Return A picklist instance containing the results of the refinement
	#
	function stepIn( $sMoniker ) {
		# A stepin simply creates a picklist from the supplied moniker with a null refinement
		$args = array( "Moniker" => $sMoniker, "Refinement" => "" );

		# If the threshold or timeout values are not default then specify them
		if ( $this->iThreshold != 0 ) {
			$args[ "Threshold" ] = $this->iThreshold;
		}
		if ( $this->iTimeout != -1 ) {
			$args[ "Timeout" ] = $this->iTimeout;
		}

		return( new Picklist( $this->soap->DoRefine( $args ) ) );
	}

	# Get a prompt set
	#
	# Parameters:
	#   sDataSetID  ID of the dataset whose prompt sets is required
	#   sPromptSet  String identifying the type of prompt e.g. "Optimal"
	#
	# Return the prompt set (i.e. array of prompt lines) identified by the name and country.
	#
	function getPromptSet( $sDataSetID, $sPromptSet ) {
		$ret = $this->soap->DoGetPromptSet( array( "Country" => $sDataSetID,
												   "PromptSet" => $sPromptSet ) )->Line;
		return( new PromptSet( $ret ) );
	}

	# Get a formatted address from layout and a moniker
	#
	# Parameters:
	#   sLayoutName     Layout name (specifies how the address should be formatted)
	#   sMoniker        Search point moniker string that represents the address
	#
	# Return the appropriate FormattedAddress object.
	#
	function getFormattedAddress( $sLayoutName, $sMoniker ) {
		$result = $this->soap->DoGetAddress( array( "Layout" => $sLayoutName, "Moniker" => $sMoniker ) );

		return( new FormattedAddress( $result ) );
	}

	# Get all layouts appropriate for a data set
	#
	# Parameters:
	#   sDataSetID  ID of the dataset whose layouts are required
	#
	# Return an array of layouts available to the server for the specified data set
	#
	function getAllLayouts( $sDataSetID ) {
		$result = $this->check_soap( $this->soap->DoGetLayouts( array( "Country" => $sDataSetID ) ) );

		if ( $result != NULL ) {
			if ( is_array( $result->Layout ) )
				return( $result->Layout );
			else
				return( array( $result->Layout ) );
		}
		else
			return( array() );
	}

	# Get example addresses for a layout
	#
	# Parameters:
	#   sDataSetID      <code>String</code> ID of the dataset for which examples are required
	#   sLayoutName     <code>String</code> name of the layout for the example
	#
	# Return an array of example addresses for the country/layout combination
	#
	function getExampleAddresses( $sDataSetID, $sLayoutName ) {
		$result = $this->check_soap( $this->soap->DoGetExampleAddresses( array( "Country" => $sDataSetID,
																				"Layout"  => $sLayoutName ) ) );
		return( new Examples( $result ) );
	}

	# Get licensing information
	#
	# Returns an array of LicensedSet objects detailling the licence state.
	#
	function getLicenceInfo() {
		$result = $this->check_soap( $this->soap->DoGetLicenseInfo() );

		if ( $result != NULL ) {
			if ( is_array( $result->LicensedSet ) )
				return( $result->LicensedSet );
			else
				return( array( $result->LicensedSet ) );
		}
		else
			return( NULL );
	}

	# Get system configuration information
	#
	# Returns an array of strings each with a line of system info
	#
	function getSystemInfo() {
		$result = $this->check_soap( $this->soap->DoGetSystemInfo() );

		if ( $result != NULL ) {
			if ( is_array( $result->SystemInfo ) )
				return( $result->SystemInfo );
			else
				return( array( $result->SystemInfo ) );
		}
		else
			return( NULL );
	}
}

/* 
 * the XI class for finding addresses 
 * the term moniker is used by QAS to signify
 * either a matching address, or list off addresses
 * (it's basically the primary key for an address, or set of 
 * addresses).
 * the basic type down search begins with a picklist,
 * then several refining picklists, then if there is only one address
 * in the picklist, a formatted address.
 * i.e. picklist("some address string") => refine_picklist($monkier) => refine_picklist($moniker) => address($moniker)
 */
class qaddress {
    # Automatic quote handling function
    #
    # If the PHP setting "magic quotes" is set, POSTed fields will come
    # through with slash prefixing. This function strips slashes from
    # input strings or arrays of string.
    #
    # Without magic quotes set, the parameter is simply returned unchanged
    #
    public static function handleslash($object) {
        if (get_magic_quotes_gpc())
            if ( is_array($object)) {
                foreach($object AS $item)
                    $aOut[] = stripslashes( $item );
                return($aOut);
            }
            else
                return(stripslashes($object));
        else
            return($object);
    }
    
    /* returns list of possible addresses */
    public static function find($address_string) {
        $qas = new QuickAddress(CONTROL_WSDL_URN);
        $qas->setEngineType(QAS_SINGLELINE_ENGINE);
        $qas->setFlatten(true);
        $addresslist = $qas->searchSingleline(DATA_ID, array(self::handleslash($address_string)), 'Optimal');
        
        return $addresslist;
    }
    
    /* returns a list of type down matches */
    public static function picklist($address_string) {
        $qas = new QuickAddress(CONTROL_WSDL_URN);
        $qas->setEngineType(QAS_TYPEDOWN_ENGINE);
        $picklist = $qas->search(DATA_ID, array(self::handleslash($address_string)), QAS_ONELINE_PROMPT)->picklist;
        
        return $picklist;
    }
    
    /* returns list of type down matches, based on a previous type down match */
    public static function refine_picklist($moniker, $address_string = "") {
        $qas = new QuickAddress(CONTROL_WSDL_URN);
        $picklist = $qas->refine($moniker, self::handleslash($address_string));
        
        return $picklist;
    }
    
    /* returns a formatted address from a typedown match */
    public static function address($moniker) {
        $qas = new QuickAddress(CONTROL_WSDL_URN);
        $tAddress = $qas->getFormattedAddress(CONTROL_LAYOUT, $moniker);

        return $tAddress->atAddressLines ? $tAddress->atAddressLines : array();
    }
    
    /* old function, returns list of possible addresses */
    public static function search($house_number, $postcode = "") {
        $qasserver = CONTROL_WSDL_URN;
        $dataid = DATA_ID;

        // Escape Input
        $inputaddress = Array(self::handleslash($house_number), self::handleslash($postcode));

        // Initialise the QuickAddress class
        $qas = new QuickAddress($qasserver);
        
        // Perform search (singleline engine, flattened picklists)
        $qas->setEngineType(QAS_SINGLELINE_ENGINE);
        $qas->setFlatten(true);
        $picklist = $qas->searchSingleline($dataid, $inputaddress, 'Optimal');
        // $error = $qas->getFaultString($error);
        
        // Is the web service up?
        if ($picklist == NULL) {
            $addresslist[0] = false;
        // Is the web service up?
        } elseif ($picklist->isTimeout) {
            $addresslist[0] = false;
        // Too many matches to return from the web service
        } elseif ($picklist->isMaxMatches) {
            $addresslist[0] = false;
        // No picklist items returned -> no match
        } elseif (!is_array($picklist->atItems) || count($picklist->atItems) == 0) {
            $addresslist[0] = false;
        } else {
            $addresslist = array();
            foreach($picklist->atItems as $address) {
                    $addresslist[] = $address->Picklist . ", " . $address->Postcode;
            }
        }
        
        return $addresslist;
    }
    
    /* old function, returns a single formatted address */
    public static function format_address($house_number, $postcode = "") {
        $qasserver = CONTROL_WSDL_URN;
        $dataid = DATA_ID;
        
        $address = array();
        /* make sure there's one and only one matching address */
        if (
            ($addresslist = self::search($house_number, $postcode)) && 
            (count($addresslist) == 1) && 
            (strlen($addresslist[0]) > 1)
        ) {
            // Initialise the QuickAddress class
            $qas = new QuickAddress($qasserver);
            
            // Perform search (singleline engine, flattened picklists)
            $qas->setEngineType(QAS_SINGLELINE_ENGINE);
            $qas->setFlatten(true);
            $picklist = $qas->searchSingleline($dataid, Array(self::handleslash($addresslist[0])), 'Optimal');
            
            // Is the web service up?
            if ($picklist == NULL) {
            // Is the web service up?
            } elseif ($picklist->isTimeout) {
            // Too many matches to return from the web service
            } elseif ($picklist->isMaxMatches) {
            // No picklist items returned -> no match
            } elseif (!is_array($picklist->atItems) || count($picklist->atItems) == 0) {
            //
            } else {
                # get address using QAS moniker
                $address_tmp = $qas->getFormattedAddress(CONTROL_LAYOUT, $picklist->atItems[0]->Moniker);
        
                # store formatted results in address array
                if (is_array($address_tmp->atAddressLines )) {
                    foreach($address_tmp->atAddressLines as $line) {
                        $address[] = $line->Line;
                    }
                }
            }
        }

        return $address;
    }
    
    public static function jstree_address($moniker) {
        $address = self::address($moniker);
        $return = array("moniker" => $moniker);
        if (is_array($address)) {
            foreach($address as $field) {
                $return[strtolower(str_replace(array(' ', '-'), '', $field->Label))] = $field->Line;
            }
        }
        
        return json_encode($return);
    }
    
    public static function jstree_picklist($address_string) {
        $picklist = self::picklist($address_string);
        $return = array();
        if (isset($picklist->atItems) && is_array($picklist->atItems)) {
            foreach ($picklist->atItems as $item) {
                $t = new stdClass();
                $t->text = $item->Picklist;
                $t->id = htmlentities($item->Moniker);
                if ($item->CanStep) {
                    $t->children = true;
                }
                $return[] = $t;
            }
        
        }
        
        return json_encode($return);
    }
    
    public static function jstree_refine_picklist($moniker) {
        $picklist = self::refine_picklist($moniker);
        $return = array();
        if (isset($picklist->atItems) && is_array($picklist->atItems)) {
            foreach ($picklist->atItems as $item) {
                $t = new stdClass();
                $t->text = $item->Picklist;
                $t->id = htmlentities($item->Moniker);
                if ($item->CanStep) {
                    $t->children = true;
                }
                
                $return[] = $t;
            }
        }
        
        return json_encode($return);
    }
    
}


?>
