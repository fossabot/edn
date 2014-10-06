<?php

include_once(SITE_MODULE."plop.php");

//define('DEFAULT_PLUGIN_PATH','plugin_wiki/');
define('ABSPATH',dirname(__FILE__).'/'); 

//Tableau contenant la liste des module
$module_list = array(array( 'state'  => false, 
							'level'  => -1,
							'name'   => "",
							'class'  => NULL));
// supression de cet élément inutil...
array_pop($module_list);


function ShowModuleList(){
	global $module_list;
	$output = "";
	//On liste simplement les module ouvert:
	$output .= '<div class="matrice"><table cellspacing="1">';
	$output .= '<tr>';
	foreach($module_list as $element) { 
		$output .= '</tr>';
	}
	$output .= '</table></div>';
	return $output;
}

function addModule($module_name) {
	// element global : 
	global $module_list;
	if (file_exists($module_file)) {
		//On inclu le module
		include_once($module_file);
	} else {
		// nothing to do ...
	}
}

class ModuleSite{
	// description du plugin
	protected $description;
	// level d'utilisation du plugin
	private $useLevel;
	// version du plugin
	public $version;
	
	/*!
	 * @brief Contructeur de base
	 * @param ---
	 * @return ---
	 */
	function ModuleSite($_nomDuModule) {
		$this->description = "---";
		$this->useLevel = 1024;
		$this->version = "00.00";
		return false;
	}


?>